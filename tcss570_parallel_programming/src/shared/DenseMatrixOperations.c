#include <stdio.h>
#include "Matrix.h"
#include "error.h"
/**
 * DenseMatrix single-threaded multiply
 */
int DenseMatrix_st_mult(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c) {
	int res;
	CHECK_ERROR_RETURN(a->nr_cols != b->nr_rows || a->nr_rows != c->nr_rows || b->nr_cols != c->nr_cols, "Invalid dimensions for matrix multiply", 1);

	int c_idx;
	for (int i = 0; i < a->nr_rows; i++) {
		for (int j = 0; j < b->nr_cols; j++) {

			c_idx = IDX2RM(i, j, c->nr_cols);
			c->data[c_idx] = 0;
			for (int k = 0; k < b->nr_rows; k++) {
				c->data[IDX2RM(i, j, c->nr_cols)] += a->data[IDX2RM(i, k, a->nr_cols)] * b->data[IDX2RM(k, j, b->nr_cols)];
				// printf("c[%d, %d] += a[%d %d] * b[%d %d] == %lf * %lf\n", i, j, i, k, k, j, a->data[IDX2RM(i, k, c->nr_cols)], b->data[IDX2RM(k, j, c->nr_cols)]);
			}
		}
	}

	return 0;
}