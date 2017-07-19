#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include "../shared/Matrix.h"
#include "../shared/error.h"
#include "../shared/DenseMatrixOperations.h"
#include "../shared/Timer.h"

int main(int argc, char **argv) {
	int res;
	DenseMatrix a, b, c;
	Timer t;

	int rank;
	int nr_procs;
	int nr_threads = atoi(argv[3]);

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nr_procs);

	if (rank == 0) {
		res = DenseMatrix_mm_read(&a, argv[1]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix a");

		res = DenseMatrix_mm_read(&b, argv[2]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix b");

		res = DenseMatrix_init(&c, a.nr_rows, b.nr_cols);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix c");
	}

	#if USE_OPEN_MP
		omp_set_dynamic(0);     		 // Explicitly disable dynamic teams
		omp_set_num_threads(nr_threads); // Use nr_threads threads for all consecutive parallel regions
	#endif

	if (rank == 0) {
		Timer_start(&t);
		MPI_Request requests[nr_procs * 2];
		MPI_Status status;

		int nr_rows[nr_procs];
		int offset = 0;
		for (int i = 0; i < nr_procs; i++) {
			nr_rows[i] = a.nr_rows / nr_procs + (i < a.nr_rows % nr_procs);
			if (i == 0) {
				continue;
			}

			int dimensions[4] = { nr_rows[i], a.nr_cols, b.nr_rows, b.nr_cols };
			res = MPI_Isend(dimensions, 4, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);
		}

		for (int i = 1; i < nr_procs; i++) {
			res = MPI_Wait(&requests[i], &status);
		}

		offset = nr_rows[0];
		for (int i = 1; i < nr_procs; i++) {
			#if FLOAT_NUMERIC
				res = MPI_Isend(&a.data[IDX2RM(offset, 0, a.nr_cols)], nr_rows[i] * a.nr_cols, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &requests[i]);
				res = MPI_Isend(b.data,                                b.nr_rows * b.nr_cols, MPI_FLOAT, i, 1, MPI_COMM_WORLD, &requests[nr_procs + i]);
			#else
				res = MPI_Isend(&a.data[IDX2RM(offset, 0, a.nr_cols)], nr_rows[i] * a.nr_cols, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &requests[i]);
				res = MPI_Isend(b.data,                                b.nr_rows * b.nr_cols, MPI_DOUBLE, i, 1, MPI_COMM_WORLD, &requests[nr_procs + i]);
			#endif

			offset += nr_rows[i];
		}

		for (int i = 1; i < nr_procs; i++) {
			res = MPI_Wait(&requests[i], &status);
			res = MPI_Wait(&requests[nr_procs + i], &status);
		}

		offset = a.nr_rows;
		a.nr_rows = nr_rows[0];
		c.nr_rows = nr_rows[0];

		res = DenseMatrix_omp_mult(&a, a.nr_cols, &b, b.nr_cols, &c, c.nr_cols);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply ab = c");
		a.nr_rows = offset;
		c.nr_rows = offset;

		offset = nr_rows[0];
		for (int i = 1; i < nr_procs; i++) {
			#if FLOAT_NUMERIC
				res = MPI_Irecv(&c.data[IDX2RM(offset, 0, c.nr_cols)], nr_rows[i] * c.nr_cols, MPI_FLOAT, i, 0, MPI_COMM_WORLD, &requests[i]);
			#else
				res = MPI_Irecv(&c.data[IDX2RM(offset, 0, c.nr_cols)], nr_rows[i] * c.nr_cols, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &requests[i]);
			#endif

			offset += nr_rows[i];
		}

		for (int i = 1; i < nr_procs; i++) {
			res = MPI_Wait(&requests[i], &status);
		}

		Timer_end(&t);
		printf("mpi_dense,%d,%d-by-%d,%d-by-%d,%d,%lf\n", nr_threads, a.nr_rows, a.nr_cols, b.nr_rows, b.nr_cols, nr_procs, Timer_dur_sec(&t));
	} else {
		int dimensions[4];
		MPI_Status status;

		res = MPI_Recv(dimensions, 4, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		res = DenseMatrix_init(&a, dimensions[0], dimensions[1]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix A");

		res = DenseMatrix_init(&b, dimensions[2], dimensions[3]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix B");

		res = DenseMatrix_init(&c, a.nr_rows, b.nr_cols);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix c");

		#if FLOAT_NUMERIC
			res = MPI_Recv(a.data, a.nr_rows * a.nr_cols, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
			res = MPI_Recv(b.data, b.nr_rows * b.nr_cols, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, &status);
		#else
			res = MPI_Recv(a.data, a.nr_rows * a.nr_cols, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
			res = MPI_Recv(b.data, b.nr_rows * b.nr_cols, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
		#endif

		res = DenseMatrix_mt_mult(&a, &b, &c, nr_threads);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply ab = c");

		#if FLOAT_NUMERIC
			res = MPI_Send(c.data, c.nr_rows * c.nr_cols, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
		#else
			res = MPI_Send(c.data, c.nr_rows * c.nr_cols, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
		#endif
	}

	MPI_Finalize();
	return 0;
}