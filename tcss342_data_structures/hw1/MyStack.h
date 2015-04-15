/*
 =============================================================

 Name:		Jesse Bannon
 Class: 	TCSS 342: Data Structures
 HW Num: 	Homework 1

 HW Desc: 	Parses orders from customers.txt and creates
           	burgers from stacks.

 =============================================================
*/

#ifndef MYSTACK_H_
#define MYSTACK_H_
#include "Ingredient.h"

typedef struct myStack_struct MyStack;

void initializeStack(MyStack** stack);
int isEmpty(MyStack** stack);
void push(MyStack** stack, const Ingredient ingredient);
Ingredient pop(MyStack** stack);
Ingredient peek(MyStack** stack);
int size(MyStack** stack);
void printStack(MyStack** stack);

#endif

