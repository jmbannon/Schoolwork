#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
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

/**
 * DenseMatrix add with result scalar
 * C = A + sB
 */ 
int DenseMatrix_add(DenseMatrix *a, int lda, DenseMatrix *b, int ldb, DenseMatrix *c, int ldc, Numeric scalar) {
	CHECK_ERROR_RETURN(a->nr_cols != b->nr_cols || a->nr_cols != c->nr_cols || a->nr_rows != b->nr_rows || a->nr_rows != c->nr_rows, "Invalid dimensions for matrix add", 1);

	for (int i = 0; i < a->nr_rows; i++) {
		for (int j = 0; j < a->nr_cols; j++) {
			c->data[IDX2RM(i, j, ldc)] = a->data[IDX2RM(i, j, lda)] + (scalar * b->data[IDX2RM(i, j, ldb)]);
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
	return DenseMatrix_add(a, a->nr_cols, b, b->nr_cols, c, c->nr_cols, 1.0);
}

int DenseMatrix_st_sub(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c) {
	return DenseMatrix_add(a, a->nr_cols, b, b->nr_cols, c, c->nr_cols, -1.0);
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

		info[i] = (PThreadMultInfo){ .a = a, .lda = lda, .b = &t_b[i], .ldb = ldb, .c = &t_c[i], .ldc = ldc };

		t_nr_cols = (b->nr_cols / nr_threads) + (i < b->nr_cols % nr_threads);

		t_b[i] = (DenseMatrix){ .nr_rows = b->nr_rows, .nr_cols = t_nr_cols, .data = &b->data[IDX2RM(0, col_offset, ldb)] };
		t_c[i] = (DenseMatrix){ .nr_rows = c->nr_rows, .nr_cols = t_nr_cols, .data = &c->data[IDX2RM(0, col_offset, ldc)] };

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

void * DenseMatrix_mt_add_wrapper(void *info) {
	PThreadMultInfo *minfo = (PThreadMultInfo *)info;
	DenseMatrix_add(minfo->a, minfo->lda, minfo->b, minfo->ldb, minfo->c, minfo->ldc, 1.0);
	return NULL;
}

void * DenseMatrix_mt_sub_wrapper(void *info) {
	PThreadMultInfo *minfo = (PThreadMultInfo *)info;
	DenseMatrix_add(minfo->a, minfo->lda, minfo->b, minfo->ldb, minfo->c, minfo->ldc, -1.0);
	return NULL;
}

/**
 * DenseMatrix multi-threaded add/sub
 * Each thread takes a partition of a, b, and c's rows, A_i, B_i, C_i, and performs C_i = A_i +- B_i
 */
int DenseMatrix_mt_addsub_ld(DenseMatrix *a, int lda, 
	                         DenseMatrix *b, int ldb,
	                         DenseMatrix *c, int ldc,
	                         int nr_threads,
	                         bool add) {
	int res;
	CHECK_ERROR_RETURN(a->nr_cols != b->nr_cols || a->nr_cols != c->nr_cols || a->nr_rows != b->nr_rows || a->nr_rows != c->nr_rows, "Invalid dimensions for matrix add", 1);

	pthread_t thread_ids[nr_threads];
	DenseMatrix t_a[nr_threads];
	DenseMatrix t_b[nr_threads];
	DenseMatrix t_c[nr_threads];
	PThreadMultInfo info[nr_threads];

	// All dimensions are equal
	int nr_rows = a->nr_rows;
	int nr_cols = a->nr_cols;

	int active_nr_threads = 0;
	int row_offset = 0;
	int t_nr_rows;
	for (int i = 0; i < nr_threads; i++) {
		if (row_offset >= nr_rows) {
			break;
		}

		info[i] = (PThreadMultInfo){ .a = &t_a[i], .lda = lda, .b = &t_b[i], .ldb = ldb, .c = &t_c[i], .ldc = ldc };

		t_nr_rows = (a->nr_rows / nr_threads) + (i < a->nr_rows % nr_threads);

		t_a[i] = (DenseMatrix){ .nr_rows = t_nr_rows, .nr_cols = nr_cols, .data = &a->data[IDX2RM(row_offset, 0, lda)] };
		t_b[i] = (DenseMatrix){ .nr_rows = t_nr_rows, .nr_cols = nr_cols, .data = &b->data[IDX2RM(row_offset, 0, ldb)] };
		t_c[i] = (DenseMatrix){ .nr_rows = t_nr_rows, .nr_cols = nr_cols, .data = &c->data[IDX2RM(row_offset, 0, ldc)] };

		//printf("thread %d: A[%d:%d,] out of %d\n", i, row_offset, row_offset + t_nr_rows - 1, nr_rows);

		row_offset += t_nr_rows;

		if (add) {
			res = pthread_create(&thread_ids[i], NULL, DenseMatrix_mt_add_wrapper, (void *)&info[i]);
		} else { // sub
			res = pthread_create(&thread_ids[i], NULL, DenseMatrix_mt_sub_wrapper, (void *)&info[i]);
		}
		CHECK_ZERO_ERROR_RETURN(res, "Failed to create pthread");

		++active_nr_threads;
	}

	for (int i = 0; i < active_nr_threads; i++) {
		res = pthread_join(thread_ids[i], NULL);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to join pthread");
	}

	return 0;
}

int DenseMatrix_mt_add_ld(DenseMatrix *a, int lda, 
	                      DenseMatrix *b, int ldb,
	                      DenseMatrix *c, int ldc,
	                      int nr_threads) {
	return DenseMatrix_mt_addsub_ld(a, lda, b, ldb, c, ldc, nr_threads, true);
}

int DenseMatrix_mt_sub_ld(DenseMatrix *a, int lda, 
	                      DenseMatrix *b, int ldb,
	                      DenseMatrix *c, int ldc,
	                      int nr_threads) {
	return DenseMatrix_mt_addsub_ld(a, lda, b, ldb, c, ldc, nr_threads, false);
}

/**
 * DenseMatrix multi-threaded add
 */
int DenseMatrix_mt_add(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int nr_threads) {
	return DenseMatrix_mt_add_ld(a, a->nr_cols, b, b->nr_cols, c, c->nr_cols, nr_threads);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Strassen multi-thread
///////////////////////////////////////////////////////////////////////////////////////////////////////

int DenseMatrix_mt_strassen2(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int ld, int nr_threads, int min_dim) {
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
				res = DenseMatrix_mt_strassen2(&a_quad[x][y], &b_quad[x][y], &c_quad[x][y], ld, nr_threads, min_dim);
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

			res = DenseMatrix_add(&t1, half_dim, &t2, half_dim, &c_quad[x][y], ld, 1.0);
			CHECK_ZERO_ERROR_RETURN(res, "Failed to add");
		}
	}

	return 0;
}

/**
 * DenseMatrix multi-threaded multiply with strassen signature
 */
int DenseMatrix_mt_mult_ss(DenseMatrix *a, int lda, DenseMatrix *b, int ldb, DenseMatrix *c, int ldc, int nr_threads, int min_dim) {
	return DenseMatrix_mt_mult_ld(a, lda, b, ldb, c, ldc, nr_threads);
}

int DenseMatrix_mt_strassen(DenseMatrix *a, int lda, DenseMatrix *b, int ldb, DenseMatrix *c, int ldc, int nr_threads, int min_dim) {
	int res;
	CHECK_ERROR_RETURN(a->nr_cols != b->nr_rows || a->nr_rows != c->nr_rows || b->nr_cols != c->nr_cols, "Invalid dimensions for matrix multiply", 1);
	CHECK_ERROR_RETURN(a->nr_rows != a->nr_cols || b->nr_rows != b->nr_cols || c->nr_rows != c->nr_cols, "All matrices must be square", 1);

	DenseMatrix a_quad[2][2];
	DenseMatrix b_quad[2][2];
	DenseMatrix c_quad[2][2];

	int half_dim = a->nr_rows / 2;
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			a_quad[x][y] = (DenseMatrix){ .nr_rows = half_dim, .nr_cols = half_dim, .data = &a->data[IDX2RM(x * half_dim, y * half_dim, lda)] };
			b_quad[x][y] = (DenseMatrix){ .nr_rows = half_dim, .nr_cols = half_dim, .data = &b->data[IDX2RM(x * half_dim, y * half_dim, ldb)] };
			c_quad[x][y] = (DenseMatrix){ .nr_rows = half_dim, .nr_cols = half_dim, .data = &c->data[IDX2RM(x * half_dim, y * half_dim, ldc)] };
		}
	}

	DenseMatrix m[7];
	DenseMatrix t0, t1;

	for (int i = 0; i < 7; i++) {
		res = DenseMatrix_init(&m[i], half_dim, half_dim);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init m's");
	}

	res = DenseMatrix_init(&t0, half_dim, half_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init t1");

	res = DenseMatrix_init(&t1, half_dim, half_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init t1");

	// Function pointer for single or multi-threaded add/mult
	int (*DenseMatrix_mt_mult_f)(DenseMatrix *, int, DenseMatrix *, int, DenseMatrix *, int, int, int);
	if (half_dim <= min_dim) {
		DenseMatrix_mt_mult_f = DenseMatrix_mt_mult_ss;
	} else {
		DenseMatrix_mt_mult_f = DenseMatrix_mt_strassen;
	}

	/////////////////// Compute M_0

	res = DenseMatrix_mt_add_ld(&a_quad[0][0], lda, &a_quad[1][1], lda, &t0, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_0 = A_00 + A_11");

	res = DenseMatrix_mt_add_ld(&b_quad[0][0], ldb, &b_quad[1][1], ldb, &t1, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_1 = B_00 + B_11");

	res = DenseMatrix_mt_mult_f(&t0, half_dim, &t1, half_dim, &m[0], half_dim, nr_threads, min_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute M_0 = T_0 * T_1");

	printf("M0:\n");
	DenseMatrix_print(&m[0]);

	/////////////////// Compute M_1

	res = DenseMatrix_mt_add_ld(&a_quad[1][0], lda, &a_quad[1][1], lda, &t0, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_0 = A_10 + A_11");

	res = DenseMatrix_mt_mult_f(&t0, half_dim, &b_quad[0][0], ldb, &m[1], half_dim, nr_threads, min_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute M_1 = T_0 * B_11");

	/////////////////// Compute M_2

	res = DenseMatrix_mt_sub_ld(&b_quad[0][1], ldb, &b_quad[1][1], ldb, &t0, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_0 = B_01 - B_11");

	res = DenseMatrix_mt_mult_f(&a_quad[0][0], lda, &t0, half_dim, &m[2], half_dim, nr_threads, min_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute M_2 = A_00 * T0");

	/////////////////// Compute M_3

	res = DenseMatrix_mt_sub_ld(&b_quad[1][0], ldb, &b_quad[0][0], ldb, &t0, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_0 = B_10 - B_00");

	res = DenseMatrix_mt_mult_f(&a_quad[1][1], lda, &t0, half_dim, &m[3], half_dim, nr_threads, min_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute M_3 = A_11 * T0");

	printf("M3:\n");
	DenseMatrix_print(&m[3]);

	/////////////////// Compute M_4

	res = DenseMatrix_mt_add_ld(&a_quad[0][0], lda, &a_quad[0][1], lda, &t0, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_0 = A_00 + A_01");

	res = DenseMatrix_mt_mult_f(&t0, half_dim, &a_quad[1][1], lda, &m[4], half_dim, nr_threads, min_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute M_4 = T0 * A_11");

	/////////////////// Compute M_5

	res = DenseMatrix_mt_sub_ld(&a_quad[1][0], lda, &a_quad[0][0], lda, &t0, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_0 = A_10 - A_00");

	res = DenseMatrix_mt_add_ld(&b_quad[0][0], ldb, &b_quad[0][1], ldb, &t1, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_1 = B_00 + B_01");

	res = DenseMatrix_mt_mult_f(&t0, half_dim, &t1, half_dim, &m[5], half_dim, nr_threads, min_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute M_5 = T_0 * T_1");

	/////////////////// Compute M_6

	res = DenseMatrix_mt_sub_ld(&a_quad[0][1], lda, &a_quad[1][1], lda, &t0, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_0 = A_01 - A_11");

	res = DenseMatrix_mt_add_ld(&b_quad[1][0], ldb, &b_quad[1][1], ldb, &t1, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_1 = B_10 + B_11");

	res = DenseMatrix_mt_mult_f(&t0, half_dim, &t1, half_dim, &m[6], half_dim, nr_threads, min_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute M_6 = T_0 * T_1");

	printf("M6:\n");
	DenseMatrix_print(&m[6]);

	///////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////// Compute C_00

	res = DenseMatrix_mt_add_ld(&m[0], half_dim, &m[3], half_dim, &t0, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_0 = M0 + M3");

	res = DenseMatrix_mt_sub_ld(&t0, half_dim, &m[4], half_dim, &t1, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_1 = T0 - M4");

	res = DenseMatrix_mt_add_ld(&t1, half_dim, &m[6], half_dim, &c_quad[0][0], ldc, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute C_00 = T1 + M6");

	/////////////////// Compute C_01

	res = DenseMatrix_mt_add_ld(&m[2], half_dim, &m[4], half_dim, &c_quad[0][1], ldc, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute C_01 = M2 + M5");

	/////////////////// Compute C_10

	res = DenseMatrix_mt_add_ld(&m[1], half_dim, &m[3], half_dim, &c_quad[1][0], ldc, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute C_10 = M1 + M4");

	/////////////////// Compute C_11

	res = DenseMatrix_mt_sub_ld(&m[0], half_dim, &m[1], half_dim, &t0, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_0 = M0 - M1");

	res = DenseMatrix_mt_add_ld(&t0, half_dim, &m[2], half_dim, &t1, half_dim, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute T_1 = T0 + M3");

	res = DenseMatrix_mt_add_ld(&t1, half_dim, &m[6], half_dim, &c_quad[1][1], ldc, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to compute C_11 = T1 + M6");

	return 0;
}