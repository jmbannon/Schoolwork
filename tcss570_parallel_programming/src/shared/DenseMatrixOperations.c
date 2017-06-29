#include <stdio.h>
#include <pthread.h>
#include "Matrix.h"
#include "error.h"

int DenseMatrix_mult(DenseMatrix *a, int lda, DenseMatrix *b, int ldb, DenseMatrix *c, int ldc) {
	int res;
	CHECK_ERROR_RETURN(a->nr_cols != b->nr_rows || a->nr_rows != c->nr_rows || b->nr_cols != c->nr_cols, "Invalid dimensions for matrix multiply", 1);

	int c_idx;
	for (int i = 0; i < a->nr_rows; i++) {
		for (int j = 0; j < b->nr_cols; j++) {

			c_idx = IDX2RM(i, j, ldc);
			c->data[c_idx] = 0;
			for (int k = 0; k < b->nr_rows; k++) {
				c->data[c_idx] += a->data[IDX2RM(i, k, lda)] * b->data[IDX2RM(k, j, ldb)];
				// printf("c[%d, %d] += a[%d %d] * b[%d %d] == %lf * %lf\n", i, j, i, k, k, j, a->data[IDX2RM(i, k, c->nr_cols)], b->data[IDX2RM(k, j, c->nr_cols)]);
			}
		}
	}

	return 0;
}

/**
 * DenseMatrix single-threaded multiply
 */
int DenseMatrix_st_mult(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c) {
	return DenseMatrix_mult(a, a->nr_cols, b, b->nr_cols, c, c->nr_cols);
}

typedef struct _PThreadMultInfo {
	DenseMatrix *a; int lda;
	DenseMatrix *b; int ldb;
	DenseMatrix *c; int ldc;
} PThreadMultInfo;

void DenseMatrix_mt_mult_wrapper(void *info) {
	PThreadMultInfo *minfo = (PThreadMultInfo *)info;
	DenseMatrix_mult(minfo->a, minfo->lda, minfo->b, minfo->ldb, minfo->c, minfo->ldc);
}

/**
 * DenseMatrix multi-threaded multiply
 *
 * Each thread takes a partition of b's columns.
 */
int DenseMatrix_mt_mult(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int nr_threads) {
	int res;
	CHECK_ERROR_RETURN(a->nr_cols != b->nr_rows || a->nr_rows != c->nr_rows || b->nr_cols != c->nr_cols, "Invalid dimensions for matrix multiply", 1);

	pthread_t thread_ids[nr_threads];
	DenseMatrix t_b[nr_threads];
	DenseMatrix t_c[nr_threads];
	PThreadMultInfo info[nr_threads];

	int active_nr_threads = 0;
	int col_offset = 0;
	int t_nr_cols;
	for (int i = 0; i < nr_threads; i++) {
		if (col_offset >= b->nr_cols) {
			break;
		}

		info[i].a = a;
		info[i].lda = a->nr_cols;
		info[i].b = &t_b[i];
		info[i].ldb = b->nr_cols;
		info[i].c = &t_c[i];
		info[i].ldc = b->nr_cols;

		t_nr_cols = (b->nr_cols / nr_threads) + (i < b->nr_cols % nr_threads);

		t_b[i].nr_rows = b->nr_rows;
		t_b[i].nr_cols = t_nr_cols;
		t_b[i].data = &b->data[IDX2RM(0, col_offset, b->nr_cols)];

		t_c[i].nr_rows = c->nr_rows;
		t_c[i].nr_cols = t_nr_cols;
		t_c[i].data = &c->data[IDX2RM(0, col_offset, c->nr_cols)];

		// printf("thread %d: B[,%d:%d] out of %d\n", i, col_offset, col_offset + t_nr_cols - 1, b->nr_cols);

		col_offset += t_nr_cols;

		res = pthread_create(&thread_ids[i], NULL, (void *)DenseMatrix_mt_mult_wrapper, (void *)&info[i]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to create pthread");

		++active_nr_threads;
	}

	for (int i = 0; i < active_nr_threads; i++) {
		res = pthread_join(thread_ids[i], NULL);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to join pthread");
	}

	return 0;
}