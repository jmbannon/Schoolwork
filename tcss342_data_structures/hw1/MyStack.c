/*
 =============================================================

 Name:		Jesse Bannon
 Class: 	TCSS 342: Data Structures
 HW Num: 	Homework 1

 HW Desc: 	Parses orders from customers.txt and creates
           	burgers from stacks.

 =============================================================
*/

#include <stdio.h>
#include <stdlib.h>
#include "MyStack.h"
#include "Ingredient.h"

typedef struct myStack_node Node;

struct myStack_node
{
    Ingredient ing;
    Node* next;
};

struct myStack_struct
{
    int size;
    Node* top;       
};

void initializeStack(MyStack** stack) 
{
    *stack = malloc(sizeof(MyStack));
    (*stack)->size = 0;
    (*stack)->top = NULL;
}

int isEmpty(MyStack** stack) 
{
    return (*stack)->top == NULL;
}

void push(MyStack** stack, const Ingredient ingredient) 
{
    ++(*stack)->size;
    Node* topNode = malloc(sizeof(Node));
    topNode->ing = ingredient;
    topNode->next = (*stack)->top;
    (*stack)->top = topNode; 
}

Ingredient pop(MyStack** stack) 
{
    Ingredient thePop;

    if (!isEmpty(&(*stack)))
    {
        thePop = (*stack)->top->ing;
        --(*stack)->size;
        (*stack)->top = (*stack)->top->next;
    } else
        thePop = DNE;

    return thePop;   
}

Ingredient peek(MyStack** stack)
{
    return (*stack)->top->ing;
}

int size(MyStack** stack)
{
    return (*stack)->size;
}

void printStack(MyStack** stack)
{
    MyStack* tempStack;
    initializeStack(&tempStack);

    Node* temp = (*stack)->top;

    printf("[");
    while (!isEmpty(stack))
    {
        printf("%s", getIngredientString(peek(stack)));
        push(&tempStack, pop(stack));
        if (!isEmpty(stack)) printf(", ");       
    }
    printf("]\n");
    
    while (!isEmpty(&tempStack))
    {
        push(stack, pop(&tempStack));
    }
    free(tempStack);
}

