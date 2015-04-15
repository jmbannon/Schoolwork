# TCSS 372 - Computer Architecture
CC=gcc

all: baron_burgers

baron_burgers: Main.o Ingredient.o MyStack.o Burger.o
	$(CC) -o baron_burgers Main.o Ingredient.o MyStack.o Burger.o

Main.o: Main.c
	$(CC) -c Main.c

Ingredient.o: Ingredient.c Ingredient.h
	$(CC) -c Ingredient.c

MyStack.o: MyStack.c MyStack.h
	$(CC) -c MyStack.c

Burger.o: Burger.c Burger.h
	$(CC) -c Burger.c

#Need dis

