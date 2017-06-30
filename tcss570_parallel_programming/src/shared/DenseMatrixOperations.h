#ifndef __DENSE_MATRIX_OPERATIONS_H__
#define __DENSE_MATRIX_OPERATIONS_H__

#include "Matrix.h"

int DenseMatrix_st_mult(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c);

int DenseMatrix_st_add(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c);

int DenseMatrix_mt_mult(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int nr_threads);

int DenseMatrix_mt_add(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int nr_threads);

int DenseMatrix_mt_strassen(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int ld, int nr_threads, int min_dim);

#endif