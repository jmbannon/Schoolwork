#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "Matrix.h"
#include "error.h"
#include "mmio.h"

int SparseMatrix_init_mem(SparseMatrix *m) {
	m->data = malloc(m->size * sizeof(Numeric));
	CHECK_MALLOC_RETURN(m->data);

	m->row = malloc(m->size * sizeof(int));
	CHECK_MALLOC_RETURN(m->row);

	m->col = malloc(m->size * sizeof(int));
	CHECK_MALLOC_RETURN(m->col);

	return 0;
}

int SparseMatrix_init(SparseMatrix *m, int nr_rows, int nr_cols, int size, int nr_elems) {
	m->nr_rows = nr_rows;
	m->nr_cols = nr_cols;
	m->size = size;
	m->nr_elems = nr_elems;

	return SparseMatrix_init_mem(m);
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
int SparseMatrix_parse_mm_sparse(SparseMatrix *m, FILE *file) {
	int res = 0;
    int i = 0;
	while (i < m->nr_elems && res != EOF) {
		#if FLOAT_NUMERIC
			res = fscanf(file, "%d %d %f\n", &m->row[i], &m->col[i], &m->data[i]);
		#else
			res = fscanf(file, "%d %d %lf\n", &m->row[i], &m->col[i], &m->data[i]);
		#endif
		if (res == 3) {
			i++;
		}
	}

	if (file != stdin) {
		fclose(file);
	}

	CHECK_ERROR_RETURN(i != m->nr_elems, "Failed to parse read number of sparse elements", 1);
	return 0;
}

int SparseMatrix_mm_read(SparseMatrix *m, char *file_name) {
	int res;
	MM_typecode matcode;
	FILE *file;

	file = fopen(file_name, "r");
	CHECK_NULL_RETURN(file);

	res = mm_read_banner(file, &matcode);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read matrix code");

	CHECK_ERROR_RETURN(!mm_is_matrix(matcode), "File is not a matrix", 1);

	if (mm_is_sparse(matcode)) {
		res = mm_read_mtx_crd_size(file, &m->nr_rows, &m->nr_cols, &m->size);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to read sparse mm dimensions");

		m->nr_elems = m->size;

		// Initialzie matrix to zero to fill in with sparse elements
		res = SparseMatrix_init_mem(m);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to allocate memory for mm matrix");

		res = SparseMatrix_parse_mm_sparse(m, file);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to parse mm file");
	} else {
		ERROR("mm matrix code is not supported. Only supports sparse matrices");
	}

	return 0;
}