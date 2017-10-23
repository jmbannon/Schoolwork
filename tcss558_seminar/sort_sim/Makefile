CC = g++
EXEC = sorting-algos

all: compile_link clean

compile_link: compile
		$(CC) -o $(EXEC) *.o

compile:
		$(CC) -c $(wildcard *.cpp)

clean:
	rm -f ./*.o