#ifndef __MATRIX_H__
#define __MATRIX_H__

#define USE_OPEN_MP 1

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

typedef struct _SparseMatrix {
	int *row;
	int *col;
	Numeric *data;

	int size;
	int nr_elems;


	int nr_rows;
	int nr_cols;
} SparseMatrix;

int DenseMatrix_print(DenseMatrix *mat);

int DenseMatrix_print_ld(DenseMatrix *mat, int ld);

int DenseMatrix_mm_read(DenseMatrix *m, char *file_name);

int DenseMatrix_mm_read_strassen(DenseMatrix *m, char *file_name, int *nr_rows, int *nr_cols);

int DenseMatrix_init(DenseMatrix *m, int nr_rows, int nr_cols);

int DenseMatrix_init_diag(DenseMatrix *m, int nr_rows, int nr_cols, Numeric val);

int DenseMatrix_init_identity(DenseMatrix *m, int nr_rows, int nr_cols);

int DenseMatrix_init_seq_row(DenseMatrix *m, int nr_rows, int nr_cols);

int DenseMatrix_init_seq_col(DenseMatrix *m, int nr_rows, int nr_cols);

int SparseMatrix_init_mem(SparseMatrix *m);

int SparseMatrix_init(SparseMatrix *m, int nr_rows, int nr_cols, int size);

int SparseMatrix_mm_read(SparseMatrix *m, char *file_name);

#endif