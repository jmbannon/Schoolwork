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

#ifndef MALLOK_H_
#define MALLOK_H_

void create_pool(int size);
void *my_malloc(int size);
void my_free(void *block);

#endif /* MALLOK_H_ */
