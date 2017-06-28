#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "Matrix.h"
#include "error.h"
#include "mmio.h"

int DenseMatrix_print(DenseMatrix *mat) {
	const int max_width = 7;
    int idx;
    for (int i = 0; i < mat->nr_rows; i++) {
        for (int j = 0; j < mat->nr_cols; j++) {
            idx = IDX2RM(i, j, mat->nr_cols);
            printf("%*.3f ", max_width, mat->data[idx]);
        }
        printf("\n");
    }
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

int DenseMatrix_parse_mm_dense(DenseMatrix *m, FILE *file) {
	ERROR("Parsing dense mm matrices is not supported");
}

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
			m->data[IDX2RM(row, col, m->nr_cols)] = val;
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
