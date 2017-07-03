#include <stdio.h>
#include <stdlib.h>
#include "../shared/Matrix.h"
#include "../shared/error.h"
#include "../shared/DenseMatrixOperations.h"
#include "../shared/Timer.h"

int main(int argc, char **argv) {
	int res;
	DenseMatrix a, b, c;
	int a_nr_rows, a_nr_cols;
	int b_nr_rows, b_nr_cols;

	Timer t;

	int nr_threads = atoi(argv[3]);
	int min_dim = atoi(argv[4]);

	res = DenseMatrix_mm_read_strassen(&a, argv[1], &a_nr_rows, &a_nr_cols);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix a");

	res = DenseMatrix_mm_read_strassen(&b, argv[2], &b_nr_rows, &b_nr_cols);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix b");

	res = DenseMatrix_init(&c, a.nr_rows, b.nr_cols);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix c");

	Timer_start(&t);
	res = DenseMatrix_mt_strassen(&a, a.nr_cols, &b, b.nr_cols, &c, c.nr_cols, nr_threads, min_dim);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply ab = c");
	Timer_end(&t);

	printf("strassen_mthread,%d,%d-by-%d,%d-by-%d,%d,%lf\n", nr_threads, a.nr_rows, a.nr_cols, b.nr_rows, b.nr_cols, min_dim, Timer_dur_sec(&t));

	return 0;
}