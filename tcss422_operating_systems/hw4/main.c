#include <stdio.h>
#include <stdlib.h>
#include "page_manager.h"

typedef struct {
	bool done;
	unsigned int pid;
	unsigned int virtual_address;
} access;

access access1();
access access2();
access access3();
access access4();

typedef struct {
	memory_config config;
	access (*access_func)();
} simulation;

static simulation simulations[] = {{{12, 16, 9, 1}, access1},
                                   {{14, 10, 7, 1}, access2},
                                   {{10, 16, 6, 2}, access3},
                                   {{24, 24, 11, 2}, access4}};

#define FIRST_SIMULATION_TO_RUN 3
#define LAST_SIMULATION_TO_RUN 3

void run_simulation(simulation sim) {
	printf("Running simulation for the following memory configuration...\n");
	printf("Physical address space: %u bits\n", sim.config.physical_address_space);
	printf(" Virtual address space: %u bits\n", sim.config.virtual_address_space);
	printf("             Page size: %u bits\n", sim.config.page_size);
	printf("             Processes: %u\n", sim.config.processes);
	printf("(pid, virt addr) -->  phys addr\n");

	initialize_page_manager(sim.config);
	while (true) {
		access acc = sim.access_func();
		if (acc.done)
			break;
		access_result result = access_memory(acc.pid, acc.virtual_address);
		printf("(%u, %11u) --> %10u  %s\n", acc.pid, acc.virtual_address, result.physical_address, result.page_fault ? "(page fault)" : "");
	}
	destroy_page_manager();
}

int main(int argc, char* argv[]) {
	int i;
	for (i = FIRST_SIMULATION_TO_RUN; i <= LAST_SIMULATION_TO_RUN; i++)
		run_simulation(simulations[i]);

	return EXIT_SUCCESS;
}

access access1() {
	static int i = 0;
	return (access){i == 96, 0, i++ * 128};
}

access access2() {
	static int i = 0;
	return (access){i == 32, 0, i++ * 320 % 1024};
}

access access3() {
	static int i = 0;
	access acc = {i == 32, i / 2 % 2, (i / 4 * 2 + i % 2) * 64 - i % 4 / 3};
	i++;
	return acc;
}

access access4() {
	static int i = 0;
	access acc = {i == 16512, i % 2, i * (i % 2 + 1) * 512 % 16777216};
	i++;
	return acc;
}
