#include <stdio.h>
#include <stdlib.h>
#include "../shared/Matrix.h"
#include "../shared/error.h"
#include "../shared/DenseMatrixOperations.h"
#include "../shared/Timer.h"

int main(int argc, char **argv) {
	int res;
	DenseMatrix a, b, c;
	Timer t;

	int nr_threads = atoi(argv[3]);

	res = DenseMatrix_mm_read(&a, argv[1]);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix a");

	res = DenseMatrix_mm_read(&b, argv[2]);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix b");

	res = DenseMatrix_init(&c, a.nr_rows, b.nr_cols);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix c");


	Timer_start(&t);
	res = DenseMatrix_mt_mult(&a, &b, &c, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply ab = c");
	Timer_end(&t);
	
	printf("multi_thread,%d,%d-by-%d,%d-by-%d,%lf\n", nr_threads, a.nr_rows, a.nr_cols, b.nr_rows, b.nr_cols, Timer_dur_sec(&t));

	return 0;
}