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
#include "Ingredient.h"

char* getIngredientString(const Ingredient ing) {
    switch(ing)
    {
        case BEEF:	  return "Beef";
        case CHICKEN:	  return "Chicken";
        case VEGGIE:	  return "Veggie";
        
        case CHEDDAR:	  return "Cheddar";
        case MOZZARELLA:  return "Mozzarella";
        case PEPPERJACK:  return "Pepperjack";
 
        case LETTUCE:	  return "Lettuce";
        case TOMATO:	  return "Tomato";
        case ONIONS:	  return "Onions";
        case PICKLE:	  return "Pickle";
        case MUSHROOMS:	  return "Mushrooms";
  
        case KETCHUP:	  return "Ketchup";
        case MUSTARD:	  return "Mustard";
        case MAYONNAISE:  return "Mayonnaise";
        case BARON_SAUCE: return "Baron Sauce";

        case TOP_BUN:	  return "Top Bun";
        case BOTTOM_BUN:  return "Bottom Bun";
       
        default: 	  return "DNE"; 
    }
}

Ingredient getIngredient(char* ingStr) {
    int i;
    size_t len = strlen(ingStr);
    char* ingredient = malloc(len+1);

    for (i = 0; i < len; i++)
    {
        ingredient[i] = tolower(ingStr[i]);
    }
    ingredient[len] = '\0';

    if (strncmp(ingredient, "pickle", 6) == 0)
        return PICKLE;

    else if (strncmp(ingredient, "top_bun", 7) == 0)
        return TOP_BUN;

    else if (strncmp(ingredient, "mayonnaise", 10) == 0)
        return MAYONNAISE;

    else if (strncmp(ingredient, "baron-sauce", 11) == 0)
        return BARON_SAUCE;

    else if (strncmp(ingredient, "lettuce", 7) == 0)
        return LETTUCE;

    else if (strncmp(ingredient, "tomato", 6) == 0)
        return TOMATO;

    else if (strncmp(ingredient, "onions", 6) == 0)
        return ONIONS;

    else if (strncmp(ingredient, "pepperjack", 10) == 0)
        return PEPPERJACK;

    else if (strncmp(ingredient, "mozzarella", 10) == 0)
        return MOZZARELLA;

    else if (strncmp(ingredient, "cheddar", 7) == 0)
        return CHEDDAR;

    else if (strncmp(ingredient, "beef", 4) == 0)
        return BEEF;

    else if (strncmp(ingredient, "chicken", 7) == 0)
        return CHICKEN;

    else if (strncmp(ingredient, "veggie", 6) == 0)
        return VEGGIE;

    else if (strncmp(ingredient, "mushrooms", 9) == 0)
        return MUSHROOMS;

    else if (strncmp(ingredient, "mustard", 7) == 0)
        return MUSTARD;

    else if (strncmp(ingredient, "ketchup", 7) == 0)
        return KETCHUP;

    else if (strncmp(ingredient, "bottom_bun", 10) == 0)
        return BOTTOM_BUN;

    else
        return DNE;
}
