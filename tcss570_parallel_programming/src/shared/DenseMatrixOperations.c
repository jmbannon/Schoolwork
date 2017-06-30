#include <stdio.h>
#include <pthread.h>
#include "Matrix.h"
#include "error.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Pthreading
///////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct _PThreadMultInfo {
	DenseMatrix *a; int lda;
	DenseMatrix *b; int ldb;
	DenseMatrix *c; int ldc;
} PThreadMultInfo;

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Elementary Operators (add, mult)
///////////////////////////////////////////////////////////////////////////////////////////////////////

int DenseMatrix_add(DenseMatrix *a, int lda, DenseMatrix *b, int ldb, DenseMatrix *c, int ldc) {
	CHECK_ERROR_RETURN(a->nr_cols != b->nr_cols || a->nr_cols != c->nr_cols || a->nr_rows != b->nr_rows || a->nr_rows != c->nr_rows, "Invalid dimensions for matrix add", 1);

	for (int i = 0; i < a->nr_rows; i++) {
		for (int j = 0; j < a->nr_cols; j++) {
			c->data[IDX2RM(i, j, ldc)] = a->data[IDX2RM(i, j, lda)] + b->data[IDX2RM(i, j, ldb)];
		}
	}

	return 0;
}

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

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Single-thread wrappers
///////////////////////////////////////////////////////////////////////////////////////////////////////

int DenseMatrix_st_add(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c) {
	return DenseMatrix_add(a, a->nr_cols, b, b->nr_cols, c, c->nr_cols);
}

int DenseMatrix_st_mult(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c) {
	return DenseMatrix_mult(a, a->nr_cols, b, b->nr_cols, c, c->nr_cols);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Matrix-Multiply multi-thread
///////////////////////////////////////////////////////////////////////////////////////////////////////

void * DenseMatrix_mt_mult_wrapper(void *info) {
	PThreadMultInfo *minfo = (PThreadMultInfo *)info;
	DenseMatrix_mult(minfo->a, minfo->lda, minfo->b, minfo->ldb, minfo->c, minfo->ldc);
	return NULL;
}

/**
 * DenseMatrix multi-threaded multiply
 * Each thread takes a partition of b and c's columns B_i, C_i, and performs the operation provided
 * op(A, B_i, C_i)
 */
int DenseMatrix_mt_mult_ld(DenseMatrix *a, int lda, 
	                       DenseMatrix *b, int ldb,
	                       DenseMatrix *c, int ldc,
	                       int nr_threads) {
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

		res = pthread_create(&thread_ids[i], NULL, DenseMatrix_mt_mult_wrapper, (void *)&info[i]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to create pthread");

		++active_nr_threads;
	}

	for (int i = 0; i < active_nr_threads; i++) {
		res = pthread_join(thread_ids[i], NULL);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to join pthread");
	}

	return 0;
}

/**
 * DenseMatrix multi-threaded multiply
 */
int DenseMatrix_mt_mult(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int nr_threads) {
	return DenseMatrix_mt_mult_ld(a, a->nr_cols, b, b->nr_cols, c, c->nr_cols, nr_threads);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Matrix-Add multi-thread
///////////////////////////////////////////////////////////////////////////////////////////////////////

// void * DenseMatrix_mt_add_wrapper(void *info) {
// 	PThreadMultInfo *minfo = (PThreadMultInfo *)info;
// 	DenseMatrix_add(minfo->a, minfo->lda, minfo->b, minfo->ldb, minfo->c, minfo->ldc);
// 	return NULL;
// }

// /**
//  * DenseMatrix multi-threaded add with leading dimensions
//  */
// int DenseMatrix_mt_add_ld(DenseMatrix *a, int lda,
// 	                       DenseMatrix *b, int ldb,
// 	                       DenseMatrix *c, int ldc,
// 	                       int nr_threads) {
// 	return DenseMatrix_mt_op(a, lda, b, ldb, c, ldc, nr_threads, DenseMatrix_mt_add_wrapper);
// }

// /**
//  * DenseMatrix multi-threaded multiply
//  */
// int DenseMatrix_mt_add(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int nr_threads) {
// 	return DenseMatrix_mt_add_ld(a, a->nr_cols, b, b->nr_cols, c, c->nr_cols, nr_threads);
// }

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Strassen multi-thread
///////////////////////////////////////////////////////////////////////////////////////////////////////

int DenseMatrix_mt_strassen(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int ld, int nr_threads, int min_dim) {
	int res;
	CHECK_ERROR_RETURN(a->nr_cols != b->nr_rows || a->nr_rows != c->nr_rows || b->nr_cols != c->nr_cols, "Invalid dimensions for matrix multiply", 1);
	CHECK_ERROR_RETURN(a->nr_rows != a->nr_cols || b->nr_rows != b->nr_cols || c->nr_rows != c->nr_cols, "All matrices must be square", 1);

	DenseMatrix a_quad[2][2];
	DenseMatrix b_quad[2][2];
	DenseMatrix c_quad[2][2];

	int half_dim = a->nr_rows / 2;
	int half_idx;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			half_idx = IDX2RM(x * half_dim, y * half_dim, ld);
			a_quad[x][y] = (DenseMatrix){ .nr_rows = half_dim, .nr_cols = half_dim, .data = &a->data[half_idx] };
			b_quad[x][y] = (DenseMatrix){ .nr_rows = half_dim, .nr_cols = half_dim, .data = &b->data[half_idx] };
			c_quad[x][y] = (DenseMatrix){ .nr_rows = half_dim, .nr_cols = half_dim, .data = &c->data[half_idx] };

			if (half_dim == min_dim) {
				res = DenseMatrix_mt_strassen(&a_quad[x][y], &b_quad[x][y], &c_quad[x][y], ld, nr_threads, min_dim);
				CHECK_ZERO_ERROR_RETURN(res, "Recursive strassen call failed");
			}
		}
	}

	DenseMatrix t1, t2;
	res = DenseMatrix_init(&t1, half_dim, half_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init t1");

	res = DenseMatrix_init(&t2, half_dim, half_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init t1");

	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			res = DenseMatrix_mult(&a_quad[x][0], ld, &b_quad[0][y], ld, &t1, half_dim);
			CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply");

			res = DenseMatrix_mult(&a_quad[x][1], ld, &b_quad[1][y], ld, &t2, half_dim);
			CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply");

			res = DenseMatrix_add(&t1, half_dim, &t2, half_dim, &c_quad[x][y], ld);
			CHECK_ZERO_ERROR_RETURN(res, "Failed to add");
		}
	}

	return 0;
}