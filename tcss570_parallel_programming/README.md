# Overview
Three dense matrix-matrix multiply programs.

### Compiling

Requires a 64-bit Linux system with POSIX and supports `clock_gettime()`.
Compiling with GCC or Clang will suffice. Simply `make` and it should build. If needed, please edit `Makefile` accordingly to supply your own system's compiler.

### Arguments
```
# Single-Threaded:
./multiply_sthread <m1.mtx> <m2.mtx>

# Multi-Threaded:
./multiply_pthread <m1.mtx> <m2.mtx> <# threads>

# Strassen Algorithm
./multiply_strassen <m1.mtx> <m2.mtx> <# threads> <min size 2^n>
```
