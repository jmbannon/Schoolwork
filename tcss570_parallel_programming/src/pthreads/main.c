#include <stdio.h>
#include "../shared/Matrix.h"
#include "../shared/error.h"

int main(int argc, char **argv) {
	int res;
	DenseMatrix a, b, c;

	res = DenseMatrix_mm_read(&a, argv[1]);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix a");

	res = DenseMatrix_mm_read(&b, argv[2]);
	CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix b");

	printf("Read %s and %s successfully\n", argv[1], argv[2]);
	return 0;
}