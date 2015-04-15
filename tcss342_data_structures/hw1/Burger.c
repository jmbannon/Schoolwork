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
#include <ctype.h>

#include "Burger.h"
#include "Ingredient.h"
#include "MyStack.h"

#define SETBIT(var, index) (var |= (1 << index))
#define CHECKBIT(var, index) (var & (1 << index))

struct myBurger_struct
{
    MyStack* burgerStack;    
};

void initializeBurger(Burger** burger, const int theWorks) 
{
    (*burger) = malloc(sizeof(Burger));
    initializeStack(&(*burger)->burgerStack);

    if (theWorks) 
    {
        push(&(*burger)->burgerStack, BOTTOM_BUN);
        push(&(*burger)->burgerStack, KETCHUP);
        push(&(*burger)->burgerStack, MUSTARD);
        push(&(*burger)->burgerStack, MUSHROOMS);
        push(&(*burger)->burgerStack, BEEF);
        push(&(*burger)->burgerStack, CHEDDAR);
        push(&(*burger)->burgerStack, MOZZARELLA);
        push(&(*burger)->burgerStack, PEPPERJACK);
        push(&(*burger)->burgerStack, ONIONS);
        push(&(*burger)->burgerStack, TOMATO);
        push(&(*burger)->burgerStack, LETTUCE);
        push(&(*burger)->burgerStack, BARON_SAUCE);
        push(&(*burger)->burgerStack, MAYONNAISE);
        push(&(*burger)->burgerStack, TOP_BUN);
        push(&(*burger)->burgerStack, PICKLE);
    } else
    {
        push(&(*burger)->burgerStack, BOTTOM_BUN);
        push(&(*burger)->burgerStack, BEEF);
        push(&(*burger)->burgerStack, TOP_BUN);
    }
}

void changePatties(Burger** burger, char* pattyType)
{
    Ingredient newPatty, tempIng;
    MyStack* tempStack;

    newPatty = getIngredient(pattyType);

    initializeStack(&tempStack);
    
    while(size(&(*burger)->burgerStack) > 0) 
    {
        tempIng = peek(&(*burger)->burgerStack);
        if (tempIng == BEEF
                || tempIng == CHICKEN
                || tempIng == VEGGIE)
        {
            pop(&(*burger)->burgerStack);
            push(&tempStack, newPatty);
        } else
            push(&tempStack, pop(&(*burger)->burgerStack));
    }

    while(size(&tempStack) > 0)
    {
        push(&(*burger)->burgerStack, pop(&tempStack));
    }

    free(tempStack);
}

void addPatty(Burger** burger)
{
    int pattyCount = 0;
    Ingredient newPatty, tempIng;
    MyStack* tempStack;

    initializeStack(&tempStack);
    
    while(size(&(*burger)->burgerStack) > 0) 
    {
        tempIng = peek(&(*burger)->burgerStack);
        if (tempIng == BEEF
                || tempIng == CHICKEN
                || tempIng == VEGGIE)
        {
            newPatty = tempIng;
            ++pattyCount;
        }
        push(&tempStack, pop(&(*burger)->burgerStack));
    }
    
    if (pattyCount >= 3)
        pattyCount = -1;

    while(size(&tempStack) > 0)
    {
        tempIng = peek(&tempStack);

        if (tempIng == newPatty)
            --pattyCount;
        
        else if (pattyCount == 0 
                && (tempIng == ONIONS
                || tempIng == TOMATO
                || tempIng == LETTUCE
                || tempIng == BARON_SAUCE
                || tempIng == MAYONNAISE
                || tempIng == TOP_BUN
                || tempIng == PICKLE))
        {
            push(&(*burger)->burgerStack, newPatty);
            --pattyCount;
        }

        push(&(*burger)->burgerStack, pop(&tempStack));
    }

    free(tempStack);
}

void removePatty(Burger** burger)
{
    int pattyCount = 0;
    Ingredient pattyType, tempIng;
    MyStack* tempStack;

    initializeStack(&tempStack);
    
    while(size(&(*burger)->burgerStack) > 0) 
    {
        tempIng = peek(&(*burger)->burgerStack);
        if (tempIng == BEEF
                || tempIng == CHICKEN
                || tempIng == VEGGIE)
        {
            pattyType = tempIng;
            ++pattyCount;
        }
        push(&tempStack, pop(&(*burger)->burgerStack));
    }
    
    if (pattyCount == 1)
        pattyCount = -1;

    while(size(&tempStack) > 0)
    {
        tempIng = peek(&tempStack);

        if (tempIng == pattyType)
        {
            --pattyCount;
            if (pattyCount == 0)
                pop(&tempStack);
        }

        push(&(*burger)->burgerStack, pop(&tempStack));
    }
    free(tempStack);
}

void addCategory(Burger** burger, char* catType)
{
    int i, categoryType;
    size_t len = strlen(catType);
    char* category = malloc(len+1);
    Ingredient temp;

    unsigned int ingredientBool = 0;

    MyStack* tempStack; 
    initializeStack(&tempStack);

    for (i = 0; i < len; i++) 
        category[i] = tolower(catType[i]);
    
    category[len] = '\0';

    if (strncmp(category, "cheese", 6) == 0)
        categoryType = 1;

    else if (strncmp(category, "sauce", 5) == 0) 
        categoryType = 2;

    else if (strncmp(category, "veggies", 7) == 0) 
        categoryType = 3;

    else
        return;

    while (size(&(*burger)->burgerStack) != 0) 
    {
        temp = peek(&(*burger)->burgerStack);
 
        if (categoryType == 1)
        {
            switch(temp)
            {
                case CHEDDAR:
                    ingredientBool |= 0x1; break;
                case MOZZARELLA:
                    ingredientBool |= 0x2; break;
                case PEPPERJACK:
                    ingredientBool |= 0x4; break;
                default: break;
            }                    
        } else if (categoryType == 2)
        {
            switch(temp)
            {
                case KETCHUP:
                    ingredientBool |= 0x1; break;
                case MUSTARD:
                    ingredientBool |= 0x2; break;
                case BARON_SAUCE:
                    ingredientBool |= 0x4; break;
                case MAYONNAISE:
                    ingredientBool |= 0x8; break;
                default: break;
            }
        } else if (categoryType == 3)
        {
            switch(temp)
            {
                case MUSHROOMS:
                    ingredientBool |= 0x1; break;
                case ONIONS:
                    ingredientBool |= 0x2; break;
                case TOMATO:
                    ingredientBool |= 0x4; break;
                case LETTUCE:
                    ingredientBool |= 0x8; break;
                case PICKLE:
                    ingredientBool |= 0x10; break; 
                default: break; 
            }
        }  
        push(&tempStack, pop(&(*burger)->burgerStack));                                       
    }
    
    while (size(&tempStack) > 0) 
    {
        temp = peek(&tempStack);

        if (categoryType == 1
                && (temp == BEEF
                || temp == CHICKEN
                || temp == VEGGIE))
        {
            push(&(*burger)->burgerStack, pop(&tempStack));
   
            if (!CHECKBIT(ingredientBool, 0))
                push(&(*burger)->burgerStack, CHEDDAR);

            if (!CHECKBIT(ingredientBool, 1))
                push(&(*burger)->burgerStack, MOZZARELLA);
            
            if (!CHECKBIT(ingredientBool, 2))
                push(&(*burger)->burgerStack, PEPPERJACK);

            categoryType = 0;                   
        } 
        else if (categoryType == 2)
        {
            if (temp == BOTTOM_BUN)
            {
                push(&(*burger)->burgerStack, pop(&tempStack));
           
                if (!CHECKBIT(ingredientBool, 0))
                    push(&(*burger)->burgerStack, KETCHUP);
               
                if (!CHECKBIT(ingredientBool, 1))
                    push(&(*burger)->burgerStack, MUSTARD);
            }
            else if (temp == TOP_BUN)
            {
                if (!CHECKBIT(ingredientBool, 2)) 
                    push(&(*burger)->burgerStack, BARON_SAUCE);
                
                if (!CHECKBIT(ingredientBool, 3))
                    push(&(*burger)->burgerStack, MAYONNAISE);
            }
        }
        else if (categoryType == 3)
        {
            if (!CHECKBIT(ingredientBool, 0)
                    && (temp == BEEF
                    || temp == CHICKEN
                    || temp == VEGGIE))
            {
                push(&(*burger)->burgerStack, MUSHROOMS);
                SETBIT(ingredientBool, 0);
            }

            if (temp == BARON_SAUCE
                    || temp == MAYONNAISE
                    || temp == TOP_BUN)
            {
                if (!CHECKBIT(ingredientBool, 1))
                {
                    push(&(*burger)->burgerStack, ONIONS);
                    SETBIT(ingredientBool, 1);
                }

                if (!CHECKBIT(ingredientBool, 2))
                {
                    push(&(*burger)->burgerStack, TOMATO);
                    SETBIT(ingredientBool, 2);
                }

                if (!CHECKBIT(ingredientBool, 3))
                {
                    push(&(*burger)->burgerStack, LETTUCE);
                    SETBIT(ingredientBool, 3);
                }

                if (temp == TOP_BUN && !CHECKBIT(ingredientBool, 4))
                {
                    push(&(*burger)->burgerStack, pop(&tempStack));
                    push(&(*burger)->burgerStack, PICKLE);
                    break;
                }
            }      
        }

        if (size(&tempStack) > 0)
            push(&(*burger)->burgerStack, pop(&tempStack));
    }
    free(tempStack);
    free(category);
}

void removeCategory(Burger** burger, char* catType)
{
    size_t len = strlen(catType);
    char* category = malloc(len+1);
    int i, categoryType;
    MyStack* tempStack;
    Ingredient temp;

    initializeStack(&tempStack);
    
    for (i = 0; i < len; i++) 
        category[i] = tolower(catType[i]);

    category[len] = '\0';

    if (strncmp(category, "cheese", 6) == 0)
        categoryType = 1;

    else if (strncmp(category, "sauce", 5) == 0)
        categoryType = 2;
 
    else if (strncmp(category, "veggies", 7) == 0)
        categoryType = 3;

    else
        return;

    while (size(&(*burger)->burgerStack) > 0)
    {
        temp = peek(&(*burger)->burgerStack);
        if (categoryType == 1
                && (temp == CHEDDAR
                || temp == MOZZARELLA
                || temp == PEPPERJACK))      
            pop(&(*burger)->burgerStack);

        else if (categoryType == 2
                && (temp == KETCHUP
                || temp == MUSTARD
                || temp == MAYONNAISE
                || temp == BARON_SAUCE))
            pop(&(*burger)->burgerStack);

        else if (categoryType == 3
                && (temp == LETTUCE
                || temp == TOMATO
                || temp == ONIONS
                || temp == PICKLE
                || temp == MUSHROOMS))
            pop(&(*burger)->burgerStack);

        else
            push(&tempStack, pop(&(*burger)->burgerStack));
    }

    while (size(&tempStack) > 0)
        push(&(*burger)->burgerStack, pop(&tempStack));
}

void addIngredient(Burger** burger, char* ing)
{
    size_t len = strlen(ing);
    char* tempIngredient = malloc(len+1);
    MyStack* tempStack;
    int i; 
    Ingredient temp, add;

    initializeStack(&tempStack);

    for (i = 0; i < len; i++)
        tempIngredient[i] = tolower(ing[i]);

    tempIngredient[len] = '\0';
    add = getIngredient(tempIngredient);
    
    if (add == DNE)
    {
        free(tempIngredient);
        free(tempStack);
        return;
    }

    while (size(&(*burger)->burgerStack) > 0)
    {
        temp = peek(&(*burger)->burgerStack);

        if (add < temp)
        {
            push(&tempStack, add);
            add = 99;
        }
        else if (add == temp)
            break;
        else
            push(&tempStack, pop(&(*burger)->burgerStack));
    }

    while (size(&tempStack) > 0)
        push(&(*burger)->burgerStack, pop(&tempStack));        

    free(tempStack);
    free(tempIngredient);
}

void removeIngredient(Burger** burger, char* ingredient)
{
    size_t len = strlen(ingredient);
    char* tempIngredient = malloc(len+1);
    MyStack* tempStack;
    Ingredient temp;
    int i;   

    initializeStack(&tempStack);

    for (i = 0; i < len; i++)
    {
        tempIngredient[i] = tolower(ingredient[i]);
    }

    tempIngredient[i+1] = '\0';
    temp = getIngredient(tempIngredient);
  
    if (temp == DNE)
    {
        free(tempIngredient);
        free(tempStack);
        return;
    }

    while (size(&(*burger)->burgerStack) > 0)
    {
        if (peek(&(*burger)->burgerStack) != temp)
            push(&tempStack, pop(&(*burger)->burgerStack));
   
        else
            pop(&(*burger)->burgerStack);            
    }
    
    while (size(&tempStack) > 0)
        push(&(*burger)->burgerStack, pop(&tempStack));

    free(tempStack);
    free(tempIngredient);
}

void printBurger(Burger** burger)
{
     printStack(&(*burger)->burgerStack);
}
