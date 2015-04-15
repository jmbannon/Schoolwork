/*
 =============================================================

 Name:		Jesse Bannon
 Class: 	TCSS 342: Data Structures
 HW Num: 	Homework 1

 HW Desc: 	Parses orders from customers.txt and creates
           	burgers from stacks.

 =============================================================
*/

#ifndef INGREDIENT_H_
#define INGREDIENT_H_

typedef enum ingredient_enum Ingredient;

enum ingredient_enum 
{
        DNE,
 
        PICKLE,       
        TOP_BUN,
        MAYONNAISE,
        BARON_SAUCE,
        LETTUCE,
        TOMATO,
        ONIONS,
        PEPPERJACK,
        MOZZARELLA,
        CHEDDAR,
        BEEF,
        CHICKEN,
        VEGGIE,
        MUSHROOMS,
        MUSTARD,
        KETCHUP,
        BOTTOM_BUN
};

char* getIngredientString(const Ingredient ing);
Ingredient getIngredient(char* ingStr);

#endif

