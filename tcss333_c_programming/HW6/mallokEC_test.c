/*
==================================================================
File:   mallok_test.c

Author: Jesse Bannon
Class:  TCSS 333 A
HW:     HW 6
Desc:   Creates private memory allocation and allows client to
        free or assign memory from it.
==================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "mallok.h"

void testOne();

int main(void) {

    printf("===== Test One =====\n");
    testOne();

    printf("\n===== Test Two =====\n");
    testTwo();

    printf("\n===== Test Three =====\n");
    testThree();

    return 0;
}

void testOne() {
    create_pool(1000);
    void* temp1 = my_malloc(200);
    void* temp2 = my_malloc(200);
    void* temp3 = my_malloc(200);
    void* temp4 = my_malloc(200);
    void* temp5 = my_malloc(200);

    my_free(temp3);

    void* temp6 = my_malloc(210);
    void* temp7 = my_malloc(150);
    void* temp8 = my_malloc(60);
    void* temp9 = my_malloc(50);
}

void testTwo() {
    create_pool(1000);
    void* temp1 = my_malloc(200);
    void* temp2 = my_malloc(200);
    void* temp3 = my_malloc(200);
    void* temp4 = my_malloc(200);
    void* temp5 = my_malloc(200);

    int i;
    for (i = 0; i < 200; i++) {
        *((char*)temp1+i*sizeof(char)) = 'A';
    }

    for (i = 0; i < 200; i++)
        *((char*)temp2+i*sizeof(char)) = 'B';

    printf("\n** First byte of first block = %c\n", *((char*)temp1));
    printf("** Last byte of first block = %c\n", *((char*)temp1+199));
    printf("** First byte of second block = %c\n", *((char*)temp2));
    printf("** Last byte of second block = %c\n", *((char*)temp2+199));
}

void testThree() {
    create_pool(1000);
    void* temp1 = my_malloc(1000);
    my_free(temp1);

    void* temp2 = my_malloc(250);
    void* temp3 = my_malloc(250);
    void* temp4 = my_malloc(250);
    void* temp5 = my_malloc(250);

    my_free(temp2);
    my_free(temp3);
    my_free(temp4);
    my_free(temp5);

    void* temp6 = my_malloc(100);
    void* temp7 = my_malloc(100);
    void* temp8 = my_malloc(100);
    void* temp9 = my_malloc(100);
    void* temp10 = my_malloc(100);
    void* temp11 = my_malloc(100);
    void* temp12 = my_malloc(100);
    void* temp13 = my_malloc(100);
    void* temp14 = my_malloc(100);
    void* temp15 = my_malloc(100);

    my_free(temp6);
    my_free(temp7);
    my_free(temp8);
    my_free(temp9);
    my_free(temp10);
    my_free(temp11);
    my_free(temp12);
    my_free(temp13);
    my_free(temp14);
    my_free(temp15);
}
