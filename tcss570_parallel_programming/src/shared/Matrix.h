#ifndef __MATRIX_H__
#define __MATRIX_H__

#define FLOAT_NUMERIC 0

#if FLOAT_NUMERIC
typedef float Numeric;
#else
typedef double Numeric;
#endif

#define IDX2RM(row, col, nr_cols) ((row) * (nr_cols) + (col))


typedef struct _DenseMatrix {
	Numeric *data;
	int nr_rows;
	int nr_cols;
} DenseMatrix;

int DenseMatrix_print(DenseMatrix *mat);

int DenseMatrix_print_ld(DenseMatrix *mat, int ld);

int DenseMatrix_mm_read(DenseMatrix *m, char *file_name);

int DenseMatrix_init(DenseMatrix *m, int nr_rows, int nr_cols);

int DenseMatrix_init_diag(DenseMatrix *m, int nr_rows, int nr_cols, Numeric val);

int DenseMatrix_init_identity(DenseMatrix *m, int nr_rows, int nr_cols);

int DenseMatrix_init_seq_row(DenseMatrix *m, int nr_rows, int nr_cols);

int DenseMatrix_init_seq_col(DenseMatrix *m, int nr_rows, int nr_cols);

#endif