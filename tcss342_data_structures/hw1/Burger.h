/*
 =============================================================

 Name:		Jesse Bannon
 Class: 	TCSS 342: Data Structures
 HW Num: 	Homework 1

 HW Desc: 	Parses orders from customers.txt and creates
           	burgers from stacks.

 =============================================================
*/

#ifndef BURGER_H_
#define BURGER_H_

typedef struct myBurger_struct Burger;

void initializeBurger(Burger** burger, const int theWorks);
void changePatties(Burger** burger, char* pattyType);
void addPatty(Burger** burger);
void removePatty(Burger** burger);
void addCategory(Burger** burger, char* catType);
void removeCategory(Burger** burger, char* catType);
void addIngredient(Burger** burger, char* ing);
void removeIngredient(Burger** burger, char* ingredient);
void printBurger(Burger** burger);

#endif

