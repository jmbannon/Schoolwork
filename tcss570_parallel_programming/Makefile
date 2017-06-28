
PTHREAD_EXEC = multiply_pthread
CC    = gcc

FLAGS = -fopenmp

PTHREAD_SRC = $(wildcard src/pthreads/*.c)
SHARED_SRC  = $(wildcard src/shared/*.c)

all: pthread_compile pthread_link clean

pthread_link:
	${CC} -o ${PTHREAD_EXEC} *.o

shared_compile:
	${CC} ${FLAGS} -c ${SHARED_SRC}

pthread_compile:
	${CC} ${FLAGS} -c ${PTHREAD_SRC}

clean:
	rm *.o