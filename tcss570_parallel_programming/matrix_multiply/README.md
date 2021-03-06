# Overview
Three dense matrix-matrix multiply programs.

### Compiling

Requires a 64-bit Linux system with POSIX and supports `clock_gettime()`, and a compiler which supports c99 standard, and a variant of MPI.
Recommended to compile with GCC. Simply `make` and it should build. If needed, please edit `Makefile` accordingly to supply your own system's compiler and flags.

Within the file `src/shared/Matrix.h`, the macro define `USE_OPEN_MP` toggles whether to use OpenMP (1) or Pthreads (0). The macro define `FLOAT_NUMERIC` toggles whether to use float precision (1) or double precision (0).

### Arguments
```
# Single-Threaded:
./multiply_sthread <m1.mtx> <m2.mtx>

# Multi-Threaded:
./multiply_pthread <m1.mtx> <m2.mtx> <# threads>

# Strassen Algorithm
./multiply_strassen <m1.mtx> <m2.mtx> <# threads> <min size 2^n>

# Sparse MPI Algorithm
mpirun ./multiply_mpi_sparse <m1.mtx> <m2.mtx>

# Dense MPI Algorithm
mpirun ./multiply_mpi_dense <m1.mtx> <m2.mtx>
```
