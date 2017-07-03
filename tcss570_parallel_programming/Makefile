
STHREAD_EXEC = multiply_sthread
PTHREAD_EXEC = multiply_pthread
STRASSEN_EXEC = multiply_strassen
CC    = gcc

LIBS = -fopenmp -lpthread
INCL = -I./include/iohb1.0 -L./include/iohb1.0

FLAGS = ${LIBS} ${INCL}

STHREAD_SRC = $(wildcard src/sthread/*.c)
PTHREAD_SRC = $(wildcard src/pthread/*.c)
STRASSEN_SRC = $(wildcard src/strassen/*.c)
SHARED_SRC  = $(wildcard src/shared/*.c)

all: sthread sthread_clean pthread pthread_clean strassen sthread_clean clean_objs

sthread: sthread_compile
	${CC} ${FLAGS} -o ${STHREAD_EXEC} *.o ./include/iohb1.0/mmio.o

pthread: pthread_compile
	${CC} ${FLAGS} -o ${PTHREAD_EXEC} *.o ./include/iohb1.0/mmio.o

strassen: strassen_compile
	${CC} ${FLAGS} -o ${STRASSEN_EXEC} *.o ./include/iohb1.0/mmio.o

sthread_compile:
	${CC} ${FLAGS} -c ${STHREAD_SRC} ${SHARED_SRC}

pthread_compile:
	${CC} ${FLAGS} -c ${PTHREAD_SRC} ${SHARED_SRC}

strassen_compile:
	${CC} ${FLAGS} -c ${STRASSEN_SRC} ${SHARED_SRC}

sthread_clean:
	rm -f main.o

pthread_clean:
	rm -f main.o

strassen_clean:
	rm -f main.o

clean_objs:
	rm -f *.o

clean:
	rm -f *.o ${STHREAD_EXEC} ${PTHREAD_EXEC} ${STRASSEN_EXEC}

