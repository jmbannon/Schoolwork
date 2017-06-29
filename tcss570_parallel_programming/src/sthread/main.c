#include <stdio.h>
#include "../shared/Matrix.h"
#include "../shared/error.h"
#include "../shared/DenseMatrixOperations.h"
#include "../shared/Timer.h"

int main(int argc, char **argv) {
	int res;
	DenseMatrix a, b, c;
	Timer t;

	res = DenseMatrix_mm_read(&a, argv[1]);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix a");

	res = DenseMatrix_mm_read(&b, argv[2]);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix b");

	res = DenseMatrix_init(&c, a.nr_rows, b.nr_cols);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix c");


	Timer_start(&t);
	res = DenseMatrix_st_mult(&a, &b, &c);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply ab = c");
	Timer_end(&t);

	printf("Successfully multiplied %s (%d-by-%d) and %s (%d-by-%d).\n", argv[1], a.nr_rows, a.nr_cols, argv[2], b.nr_rows, b.nr_cols);
	printf("Duration: %lf sec\n", Timer_dur_sec(&t));

	return 0;
}