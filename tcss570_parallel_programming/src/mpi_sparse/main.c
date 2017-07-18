#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include "../shared/Matrix.h"
#include "../shared/error.h"
#include "../shared/SparseMatrixOperations.h"
#include "../shared/Timer.h"

int main(int argc, char **argv) {
	int res;
	SparseMatrix a, b, c, c_merge;
	Timer t;

	int rank;
	int nr_procs;
	int nr_threads = atoi(argv[3]);

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &nr_procs);

	int max_size;

	if (rank == 0) {
		res = SparseMatrix_mm_read(&a, argv[1]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix a");

		res = SparseMatrix_mm_read(&b, argv[2]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to read DenseMatrix b");

		max_size = a.nr_elems > b.nr_elems ? a.nr_elems : b.nr_elems;

		res = SparseMatrix_init(&c_merge, a.nr_rows, b.nr_cols, max_size);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix c_merge");

		res = SparseMatrix_init(&c, a.nr_rows, b.nr_cols, max_size);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix c");
	}

	#if USE_OPEN_MP
		omp_set_dynamic(0);     		 // Explicitly disable dynamic teams
		omp_set_num_threads(nr_threads); // Use nr_threads threads for all consecutive parallel regions
	#endif

	if (rank == 0) {
		Timer_start(&t);
		MPI_Request requests[nr_procs * 6];
		MPI_Status status;

		int nr_elems[nr_procs];
		int offset = 0;
		for (int i = 0; i < nr_procs; i++) {
			nr_elems[i] = a.nr_elems / nr_procs + (i < a.nr_elems % nr_procs);
			if (i == 0) {
				continue;
			}

			int dimensions[6] = { nr_elems[i], a.nr_rows, a.nr_cols, b.nr_elems, b.nr_rows, b.nr_cols };
			res = MPI_Isend(dimensions, 6, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);
		}

		for (int i = 1; i < nr_procs; i++) {
			res = MPI_Wait(&requests[i], &status);
		}

		offset = nr_elems[0];
		for (int i = 1; i < nr_procs; i++) {
			#if FLOAT_NUMERIC
				res = MPI_Isend(&a.data[offset], nr_elems[i], MPI_FLOAT, i, 0, MPI_COMM_WORLD, &requests[0 * nr_procs + i]);
				res = MPI_Isend(b.data, b.nr_elems, MPI_FLOAT, i, 3, MPI_COMM_WORLD, &requests[3 * nr_procs + i]);
			#else
				res = MPI_Isend(&a.data[offset], nr_elems[i], MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &requests[0 * nr_procs + i]);
				res = MPI_Isend(b.data, b.nr_elems, MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &requests[3 * nr_procs + i]);
			#endif

			res = MPI_Isend(&a.row[offset], nr_elems[i], MPI_INT, i, 1, MPI_COMM_WORLD, &requests[1 * nr_procs + i]);
			res = MPI_Isend(&a.col[offset], nr_elems[i], MPI_INT, i, 2, MPI_COMM_WORLD, &requests[2 * nr_procs + i]);

			res = MPI_Isend(b.row, b.nr_elems, MPI_INT, i, 4, MPI_COMM_WORLD, &requests[4 * nr_procs + i]);
			res = MPI_Isend(b.col, b.nr_elems, MPI_INT, i, 5, MPI_COMM_WORLD, &requests[5 * nr_procs + i]);
		}

		for (int i = 1; i < nr_procs; i++) {
			for (int j = 0; j < 6; j++) {
				res = MPI_Wait(&requests[j * nr_procs + i], &status);
			}
		}

		offset = a.nr_elems;
		a.nr_elems = nr_elems[0];
		res = SparseMatrix_mult(&a, &b, &c_merge);
		a.nr_elems = offset;

		CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply ab = c");

		for (int i = 1; i < nr_procs; i++) {
			res = MPI_Irecv(&nr_elems[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, &requests[i]);
		}

		for (int i = 1; i < nr_procs; i++) {
			res = MPI_Wait(&requests[i], &status);
		}

		offset = nr_elems[0];
		for (int i = 1; i < nr_procs; i++) {
			res = MPI_Irecv(&c_merge.row[offset], nr_elems[i], MPI_INT, i, 1, MPI_COMM_WORLD, &requests[0 * nr_procs + i]);
			res = MPI_Irecv(&c_merge.col[offset], nr_elems[i], MPI_INT, i, 2, MPI_COMM_WORLD, &requests[1 * nr_procs + i]);

			#if FLOAT_NUMERIC
				res = MPI_Irecv(&c_merge.data[offset], nr_elems[i], MPI_FLOAT, i, 3, MPI_COMM_WORLD, &requests[2 * nr_procs + i]);
			#else
				res = MPI_Irecv(&c_merge.data[offset], nr_elems[i], MPI_DOUBLE, i, 3, MPI_COMM_WORLD, &requests[2 * nr_procs + i]);
			#endif

			c_merge.nr_elems += nr_elems[i];
		}

		for (int i = 1; i < nr_procs; i++) {
			for (int j = 0; j < 3; j++) {
				res = MPI_Wait(&requests[j * nr_procs + i], &status);
			}
		}

		SparseMatrix_merge(&c_merge, &c);

		Timer_end(&t);

		SparseMatrix_print(&c);

		printf("mpi_sparse,%d,%d-by-%d,%d-by-%d,%lf\n", nr_threads, a.nr_rows, a.nr_cols, b.nr_rows, b.nr_cols, Timer_dur_sec(&t));
	} else {
		int dimensions[6];
		MPI_Status status;

		res = MPI_Recv(&dimensions, 6, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);

		res = SparseMatrix_init(&a, dimensions[1], dimensions[2], dimensions[0]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix A");
		a.nr_elems = a.size;

		res = SparseMatrix_init(&b, dimensions[4], dimensions[5], dimensions[3]);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix B");
		b.nr_elems = b.size;

		max_size = a.nr_elems > b.nr_elems ? a.nr_elems : b.nr_elems;

		res = SparseMatrix_init(&c, a.nr_rows, b.nr_cols, max_size);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to init matrix c");

		#if FLOAT_NUMERIC
			res = MPI_Recv(a.data, dimensions[0], MPI_FLOAT, 0, 0, MPI_COMM_WORLD, &status);
			res = MPI_Recv(b.data, dimensions[3], MPI_FLOAT, 0, 3, MPI_COMM_WORLD, &status);
		#else
			res = MPI_Recv(a.data, dimensions[0], MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
			res = MPI_Recv(b.data, dimensions[3], MPI_DOUBLE, 0, 3, MPI_COMM_WORLD, &status);
		#endif

		res = MPI_Recv(a.row, dimensions[0], MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
		res = MPI_Recv(a.col, dimensions[0], MPI_INT, 0, 2, MPI_COMM_WORLD, &status);
		res = MPI_Recv(b.row, dimensions[3], MPI_INT, 0, 4, MPI_COMM_WORLD, &status);
		res = MPI_Recv(b.col, dimensions[3], MPI_INT, 0, 5, MPI_COMM_WORLD, &status);

		res = SparseMatrix_mult(&a, &b, &c);
		CHECK_ZERO_ERROR_RETURN(res, "Failed to multiply ab = c");

		res = MPI_Send(&c.nr_elems, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

		res = MPI_Send(c.row, c.nr_elems, MPI_INT, 0, 1, MPI_COMM_WORLD);
		res = MPI_Send(c.col, c.nr_elems, MPI_INT, 0, 2, MPI_COMM_WORLD);
		#if FLOAT_NUMERIC
			res = MPI_Send(c.data, c.nr_elems, MPI_FLOAT, 0, 3, MPI_COMM_WORLD);
		#else
			res = MPI_Send(c.data, c.nr_elems, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
		#endif
	}

	MPI_Finalize();
	return 0;
}