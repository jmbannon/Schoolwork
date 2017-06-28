#ifndef __MATRIX_H__
#define __MATRIX_H__

#define FLOAT_NUMERIC 0

#if FLOAT_NUMERIC
typedef float Numeric;
#else
typedef double Numeric;
#endif

#define IDX2RM(row, col, nr_cols) ((col) * (nr_cols) + (row))


typedef struct _DenseMatrix {
	Numeric *data;
	int nr_rows;
	int nr_cols;
} DenseMatrix;

int DenseMatrix_print(DenseMatrix *mat);

int DenseMatrix_mm_read(DenseMatrix *m, char *file_name);

#endif