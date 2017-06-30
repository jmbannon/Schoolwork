#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "Matrix.h"
#include "error.h"
#include "mmio.h"

int DenseMatrix_print_ld(DenseMatrix *mat, int ld) {
	const int max_width = 7;
    int idx;
    for (int i = 0; i < mat->nr_rows; i++) {
        for (int j = 0; j < mat->nr_cols; j++) {
            idx = IDX2RM(i, j, ld);
            printf("%*.3f ", max_width, mat->data[idx]);
        }
        printf("\n\n");
    }
}

int DenseMatrix_print(DenseMatrix *mat) {
	return DenseMatrix_print_ld(mat, mat->nr_cols);
}

int DenseMatrix_init_mem_zero(DenseMatrix *m) {
	m->data = calloc(m->nr_rows * m->nr_cols, sizeof(Numeric));
	CHECK_MALLOC_RETURN(m->data);

	return 0;
}

int DenseMatrix_init_mem(DenseMatrix *m) {
	m->data = malloc(m->nr_rows * m->nr_cols * sizeof(Numeric));
	CHECK_MALLOC_RETURN(m->data);

	return 0;
}

int DenseMatrix_init(DenseMatrix *m, int nr_rows, int nr_cols) {
	m->nr_rows = nr_rows;
	m->nr_cols = nr_cols;

	return DenseMatrix_init_mem(m);
}

int DenseMatrix_init_zero(DenseMatrix *m, int nr_rows, int nr_cols) {
	m->nr_rows = nr_rows;
	m->nr_cols = nr_cols;

	return DenseMatrix_init_mem_zero(m);
}

int DenseMatrix_init_diag(DenseMatrix *m, int nr_rows, int nr_cols, Numeric val) {
	int res;

	res = DenseMatrix_init_zero(m, nr_rows, nr_cols);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init zero matrix");

	int min = nr_rows < nr_cols ? nr_rows : nr_cols;

	for (int i = 0; i < min; i++) {
		printf("%d,%d = %d\n", i, i , IDX2RM(i, i, nr_cols));
		m->data[IDX2RM(i, i, nr_cols)] = val;
	}
	return 0;
}

int DenseMatrix_init_identity(DenseMatrix *m, int nr_rows, int nr_cols) {
	return DenseMatrix_init_diag(m, nr_rows, nr_cols, 1.0);
}

static
int DenseMatrix_init_seq(DenseMatrix *m, int nr_rows, int nr_cols, bool row) {
	int res;

	res = DenseMatrix_init(m, nr_rows, nr_cols);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix");

	int i;
	int j;
	int *seq_ptr = row ? &i : &j;

	for (i = 0; i < nr_rows; i++) {
		for (j = 0; j < nr_cols; j++) {
			m->data[IDX2RM(i, j, nr_cols)] = (Numeric)*seq_ptr; 
		}
	}

	return 0;
}

int DenseMatrix_init_seq_row(DenseMatrix *m, int nr_rows, int nr_cols) {
	return DenseMatrix_init_seq(m, nr_rows, nr_cols, true);
}

int DenseMatrix_init_seq_col(DenseMatrix *m, int nr_rows, int nr_cols) {
	return DenseMatrix_init_seq(m, nr_rows, nr_cols, false);
}

int DenseMatrix_parse_mm_dense(DenseMatrix *m, FILE *file) {
	ERROR("Parsing dense mm matrices is not supported");
}

/**
 * Reads sparse mm file into a DenseMatrix
 *
 * @param m Initialized matrix
 * @param file Sparse mm file
 * @param nr_sparse_elements Total number of elements in the sparse mm file
 *
 * @see http://math.nist.gov/MatrixMarket/mmio/c/example_read.c
 */
int DenseMatrix_parse_mm_sparse(DenseMatrix *m, FILE *file, int nr_sparse_elements) {
	int res = 0;

	int row;
	int col;
	Numeric val;

    int i = 0;
	while (i < nr_sparse_elements && res != EOF) {
		#if FLOAT_NUMERIC
			res = fscanf(file, "%d %d %f\n", &row, &col, &val);
		#else
			res = fscanf(file, "%d %d %lf\n", &row, &col, &val);
		#endif
		if (res == 3) {
			// Adjust from 1-based to 0-based
			m->data[IDX2RM(row - 1, col - 1, m->nr_cols)] = val;
			i++;
		}
	}

	if (file != stdin) {
		fclose(file);
	}

	CHECK_ERROR_RETURN(i != nr_sparse_elements, "Failed to parse read number of sparse elements", 1);
	return 0;
}

int DenseMatrix_mm_read(DenseMatrix *m, char *file_name) {
	int res;
	MM_typecode matcode;
	FILE *file;

	file = fopen(file_name, "r");
	CHECK_NULL_RETURN(file);

	res = mm_read_banner(file, &matcode);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read matrix code");

	CHECK_ERROR_RETURN(!mm_is_matrix(matcode), "File is not a matrix", 1);

	if (mm_is_sparse(matcode)) {
		int nr_sparse_elements;

		res = mm_read_mtx_crd_size(file, &m->nr_rows, &m->nr_cols, &nr_sparse_elements);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to read sparse mm dimensions");

		// Initialzie matrix to zero to fill in with sparse elements
		res = DenseMatrix_init_mem_zero(m);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to allocate memory for mm matrix");

		res = DenseMatrix_parse_mm_sparse(m, file, nr_sparse_elements);
	} else if (mm_is_dense(matcode)) {
		res = mm_read_mtx_array_size(file, &m->nr_rows, &m->nr_cols);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to read dense mm dimensions");

		res = DenseMatrix_init_mem(m);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to allocate memory for mm matrix");

		res = DenseMatrix_parse_mm_dense(m, file);
	} else {
		ERROR("mm matrix code is not supported. Only supports dense and sparse matrices");
	}
	CHECK_ZERO_ERROR_RETURN(res, "Failed to parse mm file");

	return 0;
}

/** Returns a = 2^n : max(nr_rows, nr_cols) <= a where n is as small as possible to satisfy the requirement */
int pow2dim(int nr_rows, int nr_cols) {
	int max = nr_cols > nr_rows ? nr_cols : nr_rows;
	int size = 1;

	while (size <= max) {
		size *= 2;
	}

	return size;
}

int DenseMatrix_mm_read_strassen(DenseMatrix *m, char *file_name, int *nr_rows, int *nr_cols) {
	int res;
	MM_typecode matcode;
	FILE *file;

	file = fopen(file_name, "r");
	CHECK_NULL_RETURN(file);

	res = mm_read_banner(file, &matcode);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read matrix code");

	CHECK_ERROR_RETURN(!mm_is_matrix(matcode), "File is not a matrix", 1);

	if (mm_is_sparse(matcode)) {
		int nr_sparse_elements;

		res = mm_read_mtx_crd_size(file, nr_rows, nr_cols, &nr_sparse_elements);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to read sparse mm dimensions");

		int dim = pow2dim(*nr_rows, *nr_cols);

		// Initialzie matrix to zero to fill in with sparse elements
		res = DenseMatrix_init_zero(m, dim, dim);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to allocate memory for mm matrix");

		res = DenseMatrix_parse_mm_sparse(m, file, nr_sparse_elements);
	} else if (mm_is_dense(matcode)) {
		res = mm_read_mtx_array_size(file, nr_rows, nr_cols);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to read dense mm dimensions");

		int dim = pow2dim(*nr_rows, *nr_cols);

		res = DenseMatrix_init(m, dim, dim);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to allocate memory for mm matrix");

		res = DenseMatrix_parse_mm_dense(m, file);
	} else {
		ERROR("mm matrix code is not supported. Only supports dense and sparse matrices");
	}
	CHECK_ZERO_ERROR_RETURN(res, "Failed to parse mm file");

	return 0;
}



