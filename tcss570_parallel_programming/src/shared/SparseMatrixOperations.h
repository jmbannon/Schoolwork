#ifndef __SPARSE_MATRIX_OPERATIONS_H__
#define __SPARSE_MATRIX_OPERATIONS_H__

int SparseMatrix_mult(SparseMatrix *a, SparseMatrix *b, SparseMatrix *c);

int SparseMatrix_merge(SparseMatrix *m, SparseMatrix *c);

#endif