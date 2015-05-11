#ifndef _PAGE_MANAGER_H_
#define _PAGE_MANAGER_H_

#include <stdbool.h>

typedef struct {
	unsigned int physical_address_space;  // in bits, between 2 and 32 (inclusive)
	unsigned int virtual_address_space;   // in bits, between 2 and 32 (inclusive)
	unsigned int page_size;               // in bits, between 2 and 32 (inclusive)
	unsigned int processes;               // positive
} memory_config;

typedef struct {
	unsigned int virtual_page_number;
	unsigned int physical_page_number;
	unsigned int physical_address;
	bool page_fault;
} access_result;

void initialize_page_manager(memory_config mc);
void destroy_page_manager();
access_result access_memory(unsigned int pid, unsigned int virtual_address);

#endif
