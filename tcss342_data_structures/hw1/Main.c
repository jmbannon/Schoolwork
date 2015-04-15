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
#include <string.h>
#include "MyStack.h"
#include "Burger.h"
#include "Ingredient.h"

void parseLine(char* line)
{
    Burger* burger;
    Ingredient pattyType;
    int patties = 0, i = 0, omit = -1;
    char *token;
    
    const char space[2] = " ";
    token = strtok(line, space);
 
    while (token != NULL)   
    {
        //printf("token = %s, i = %d, omit = %d\n", token, i, omit);  
        if (i == 0)
        {
            if (strcmp(token, "Single") == 0)
                patties = 1;
            else if (strcmp(token, "Double") == 0)
                patties = 2;
            else if (strcmp(token, "Triple") == 0)
                patties = 3;
            else
            {
                patties = 1; ++i; continue;
            }
        }

        else if (i == 1)
        {
            pattyType = getIngredient(token);
            if (pattyType == DNE)
            {
                pattyType = BEEF;
                ++i; continue;
            }
        }

        else if (i == 2)
        {
            if (strncmp(token, "Baron", 5) == 0)
            {
                initializeBurger(&burger, 1);
                omit = 1;
            }
            else if (strncmp(token, "Burger", 6) == 0)
            {
                initializeBurger(&burger, 0);
                omit = 0;
            }
            else
                return;
            
            if (pattyType != DNE || pattyType != BEEF)
                changePatties(&burger, getIngredientString(pattyType));   
        }

        else if (i >= 3)
        {
            while (token != NULL && !strcmp(token, "but") == 0)
            {
                //printf("add/rem = %s\n", token);
                if (omit)
                {
                    removeIngredient(&burger, token);
                    removeCategory(&burger, token);
                }
                else
                {
                    addIngredient(&burger, token);
                    addCategory(&burger, token);
                }
                token = strtok(NULL, space); 
            }
            omit = (omit) ? 0 : 1;
        }
        ++i;
        token = strtok(NULL, space);              
    }

    while (patties > 1)
    {
        addPatty(&burger);
        --patties;
    }

    printBurger(&burger);
}

void testBurger()
{
    char theLine[128];
    strcpy(theLine, "Single Burger with Veggies but no Lettuce");
    parseLine(theLine);

    strcpy(theLine, "Single Veggie Baron Burger");
    parseLine(theLine);

    strcpy(theLine, "Burger with Cheese but no Cheddar");
    parseLine(theLine);

    Burger* test;
    initializeBurger(&test, 0);

    changePatties(&test, "Chicken");
    addPatty(&test);
    addPatty(&test);
    addPatty(&test);
    addPatty(&test);
    addCategory(&test, "cheese");
    addCategory(&test, "cheese");
    addPatty(&test);
    removePatty(&test);
    addCategory(&test, "sauce");
    removePatty(&test);
    removePatty(&test);
    addCategory(&test, "veggies");
    removePatty(&test);
    changePatties(&test, "Beef");
    addCategory(&test, "veggies");
    addCategory(&test, "sauce");
    removeCategory(&test, "veggies");
    addIngredient(&test, "mushrooms");
    removeIngredient(&test, "mushrooms");
    printBurger(&test);

    free(test);
}

void testMyStack()
{
    MyStack* test;
    initializeStack(&test);

    push(&test, BOTTOM_BUN);
    push(&test, BEEF);
    pop(&test);
    push(&test, BEEF);
    if (size(&test) != 2)
        printf("ERROR; WRONG STACK SIZE\n");
    if (isEmpty(&test))
        printf("ERROR; ISEMPTY NOT WORKING\n");

    pop(&test);
    pop(&test);
    pop(&test);
    pop(&test);
 
    if (!isEmpty(&test))
        printf("ERROR; ISEMPTY NOT WORKING\n");

    free(test);
}

int main(void) {
    char readLine[128];
    int orderNum = 0;
    FILE* infile;
    size_t len;

    infile = fopen("customer.txt", "r");	
    
    //testMyStack();
    //testBurger();

    while (fgets(readLine, 128, infile) != NULL)
    {
        len = strlen(readLine) - 1;
        if (readLine[len] == '\n')
            readLine[len] = '\0';

        printf("Processing Order %d: %s\n", orderNum++, readLine);
        parseLine(readLine);
        printf("\n");
    }
    return 0;
}
