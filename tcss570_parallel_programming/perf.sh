#!/bin/bash

STHREAD_EXEC=./multiply_sthread
PTHREAD_EXEC=./multiply_pthread
STRASSEN_EXEC=./multiply_strassen

MATRICES[0]="data/nnc1374.mtx"
MATRICES[1]="data/lns_3937.mtx"

# for i in 0 1; do
#     sleep 1
#     $STHREAD_EXEC ${MATRICES[$i]} ${MATRICES[$i]}
# done

# for i in 0 1; do
#     sleep 1
#     for threads in 6 12; do
#         sleep 1
#         $PTHREAD_EXEC ${MATRICES[$i]} ${MATRICES[$i]} $threads
#     done
# done

for i in 0 1; do
    sleep 1
    for threads in 1 2 4 8 16; do
        sleep 1
        for min_size in 1024; do
            sleep 1
            $STRASSEN_EXEC ${MATRICES[$i]} ${MATRICES[$i]} $threads $min_size
        done
    done
done
