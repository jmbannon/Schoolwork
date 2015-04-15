/*
Grader Test Program
 */

#include <stdio.h>
#include <stdlib.h>
#include "mallok.h"

void testOne() {

    int count = 0;
    int i;
    create_pool(1000);
    while (my_malloc(10) != NULL) {
       count++;
    }
    if (count != 100) {
         printf("Test One FAIL: %d allocations (should be 100)\n", count);
    }
    printf("End of Test One\n");
}

void testTwo() {

    create_pool(1000);
    int reps, i;
    void *blockAddr[5];

   
    for (reps = 0; reps < 3; reps++) {
        for (i = 0; i < 5; i++) {
             blockAddr[i] = my_malloc(200);
             if (blockAddr[i] == NULL) 
                 printf("Test Two FAIL: block allocation didn't work.\n");
        }  
      
        for (i = 0; i < 5; i++) {
             my_free(blockAddr[i]);
        }
    }
    printf("End of Test Two\n");
}



void testThree() {
    create_pool(1000);
    void* temp1 = my_malloc(200);
    void* temp2 = my_malloc(200);
    void* temp3 = my_malloc(200);
    void* temp4 = my_malloc(200);
    void* temp5 = my_malloc(200);

    if (temp1 == NULL || temp2 == NULL || temp3 == NULL 
          || temp4 == NULL || temp5 == NULL) {
        printf("Test Three: FAIL, original 5 blocks\n");
    }

    my_free(temp3);

    void* temp6 = my_malloc(210);
    if (temp6 != NULL)
         printf("Test Three: FAIL, 210 block\n");

    void* temp7 = my_malloc(150);
    if (temp7 == NULL) 
         printf("Test Three: FAIL, 150 block \n");

    void* temp8 = my_malloc(60);
    if (temp8 != NULL) 
         printf("Test Three: FAIL, 60 block \n");

    void* temp9 = my_malloc(50);
    if (temp9 == NULL) 
         printf("Test Three: FAIL, 50 block \n");
    printf("End of Test Three\n");
}

void testFour() {
    create_pool(1000);
    char *temp[5];
    char c; 
    int i,j;

    for (i = 0; i < 5; i++) {
        temp[i] = my_malloc(200);
        if (temp[i] == NULL) {
            printf("Test Four: FAIL, initial 5 blocks\n");
        }
    }

    for (i = 0, c = 'A'; i < 5; i++,c++) {
        for (j = 0; j < 200; j++) {
            temp[i][j] = c;
        }
    }

    for (i = 0, c = 'A'; i < 5; i++,c++) {
        int count = 0;
        for (j = 0; j < 200; j++) {
            if (temp[i][j] == c)
                  count++;
        }
        if (count != 200) {
            printf("Test Four: FAIL - block has bad contents\n");
        }

    }
    printf("End of Test Four\n");
}

void batchOfBlocks(int size, int number) {
    int i;
    void *addr[number];

    for (i = 0; i < number; i++) {
         addr[i] = my_malloc(size);
         if (addr[i] == NULL) {
              printf("Test 5: FAIL to get block of size %d\n",size);
         }
    }

    for (i = 0; i < number; i++) {
         my_free(addr[i]);
    }

}

void testFive() {
    create_pool(1000);

    batchOfBlocks(1000,1);
    batchOfBlocks(250,4);
    batchOfBlocks(100,10);
    printf("End of Test Five\n");

}

void testSix() {
    create_pool(500);
    void *addr[5];
    int i;
    for (i = 0; i < 5; i++) {
        addr[i] = my_malloc(100);
        if (addr[i] == NULL) {
            printf("Test Six: FAIL to get original block\n");
        }
    }
    my_free(addr[0]);
    my_free(addr[2]);
    my_free(addr[4]);
    void *ptr = my_malloc(200);
    if (ptr != NULL) {
         printf("Test Six: FAIL - got a block that wasn't there (200)\n");
    }
    my_free(addr[1]);
    ptr = my_malloc(301);
    if (ptr != NULL) {
         printf("Test Six: FAIL - got a block that wasn't there (301)\n");
    }    
    ptr = my_malloc(300);
    if (ptr == NULL) {
         printf("Test Six: FAIL - to get block (300)\n");
    }    
    printf("End of Test Six\n");
}

int main(void) {

    setvbuf(stdout, NULL, _IONBF, 0);

    int userInput;

    printf("Which test to start? (1-6): ");
    scanf("%d", &userInput);

    if (!(1 <= userInput && userInput <= 6)) {
        printf("Bad Input!");
        return 1;
    }

    switch(userInput) {
    case 1:
    printf("===== Test One =====\n");
    testOne();

    case 2:
    printf("\n===== Test Two =====\n");
    testTwo();

    case 3:
    printf("\n===== Test Three =====\n");
    testThree();

    case 4:
    printf("\n===== Test Four =====\n");
    testFour();

    case 5:
    printf("\n===== Test Five =====\n");
    testFive();

    case 6:
    printf("\n===== Test Six =====\n");    
    }

    testSix();


    return 0;
}
