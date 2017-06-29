#ifndef __DENSE_MATRIX_OPERATIONS_H__
#define __DENSE_MATRIX_OPERATIONS_H__

#include "Matrix.h"

int DenseMatrix_st_mult(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c);

int DenseMatrix_mt_mult(DenseMatrix *a, DenseMatrix *b, DenseMatrix *c, int nr_threads);

#endif