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

	// res = DenseMatrix_mm_read(&a, argv[1]);
	// CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix a");

	// res = DenseMatrix_mm_read(&b, argv[2]);
	// CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix b");

	int ld = 16;
	int min_dim = 4;

	res = DenseMatrix_init_seq_row(&a, ld, ld);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init a");

	res = DenseMatrix_init_seq_row(&b, ld, ld);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init b");

	res = DenseMatrix_init(&c, a.nr_rows, b.nr_cols);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix c");


	Timer_start(&t);
	//res = DenseMatrix_mt_strassen(&a, &b, &c, ld, 1, min_dim);
	res = DenseMatrix_mt_add(&a, &b, &c, nr_threads);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply ab = c");
	Timer_end(&t);

	printf("Successfully multiplied %s (%d-by-%d) and %s (%d-by-%d).\n", argv[1], a.nr_rows, a.nr_cols, argv[2], b.nr_rows, b.nr_cols);
	printf("Duration: %lf sec\n", Timer_dur_sec(&t));
	printf("Thread Count: %d\n", nr_threads);

	DenseMatrix_print(&c);

	return 0;
}