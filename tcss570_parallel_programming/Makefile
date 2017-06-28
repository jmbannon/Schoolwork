
PTHREAD_EXEC = multiply_pthread
CC    = gcc

LIBS = -fopenmp
INCL = -I./include/iohb1.0 -L./include/iohb1.0

FLAGS = ${LIBS} ${INCL}

PTHREAD_SRC = $(wildcard src/pthreads/*.c)
SHARED_SRC  = $(wildcard src/shared/*.c)

pthread: shared_compile pthread_compile pthread_link clean

pthread_link:
	${CC} -o ${PTHREAD_EXEC} *.o ./include/iohb1.0/mmio.o

shared_compile:
	${CC} ${FLAGS} -c ${SHARED_SRC}

pthread_compile:
	${CC} ${FLAGS} -c ${PTHREAD_SRC}

clean:
	rm -f *.o