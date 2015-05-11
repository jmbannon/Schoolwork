/*
===============================================================================
*                                                                             *
* Author:      Jesse Bannon                                                   *
* Date:        04/25/15                                                       *
* Class:       TCSS 422: Operating Systems                                    *
* School:      University of Washington Tacoma                                *
* Desc:        Simulates virtual address translation to a physical address    *
*              using a single level page table.                               *
* Copyright:   Use for educational purposes only.                             *
*                                                                             *
===============================================================================
*/

#include <stdlib.h>
#include <stdio.h>
#include "page_manager.h"
#define POWER_TWO(bit_offset) (unsigned int)(0x0 | (0x1 << bit_offset))

/*
===============================================================================
= Structures ==================================================================
===============================================================================
*/

typedef struct _page_entry {
    unsigned int phys_page;
    bool valid;
} page_entry;

typedef struct _frame_entry {
    unsigned int RU, pid;
    bool in_use;
} frame_entry; 

struct _translation_info {
    unsigned int virt_entries;
    unsigned int phys_frames;
} trans_info;

/*
===============================================================================
= Global Variables ============================================================
===============================================================================
*/

static unsigned int time;
static memory_config config;
static page_entry ** table;
static frame_entry * frame_table;

/*
===============================================================================
= Functions ===================================================================
===============================================================================
*/

/* 
 * Translates a virtual address to a physical address.
 */
unsigned int get_physical_address(unsigned int virt_address,
                                  unsigned int phys_page_num,
                                  unsigned int offset_size)
{
    unsigned int bit_buffer = (phys_page_num << offset_size);
    bit_buffer |= (virt_address % POWER_TWO(offset_size));
    return bit_buffer;
}

/*
 * Initializes the virtual page(s) for each process and initializes
 * the physical page frame table.
 */
void initialize_page_manager(memory_config mc)
{
    unsigned int i;
    time = 0;
    trans_info.virt_entries 
            = POWER_TWO(mc.virtual_address_space - mc.page_size);
    trans_info.phys_frames
            = POWER_TWO(mc.physical_address_space - mc.page_size);
    config = mc;

    table = calloc(mc.processes, sizeof(page_entry *));
    frame_table = calloc(trans_info.phys_frames, sizeof(frame_entry));
    if (table == NULL || frame_table == NULL)
        return;

    for (i = 0; i < mc.processes; i++) {
        table[i] = calloc(trans_info.virt_entries, sizeof(page_entry));
        if (table[i] == NULL)
            return;
    }
}

/*
 * Frees the memory used for the page tables.
 */
void destroy_page_manager() 
{
    unsigned int i;
    for (i = 0; i < config.processes; i++)
        free(table[i]);
    free(table);
    free(frame_table);
}

/*
 * Pages the page table with the virtual address and process id given.
 * If the table entry is valid it is a hit and returns the result.
 * Otherwise it walks the page table searching for a frame that is
 * not in use.
 *
 * If one is found, the physical page is assigned to the virtual address'
 * page entry.  Otherwise, the physical page that is least recently used
 * and has a corresponding pid is assigned.
 */
access_result access_memory(unsigned int pid, 
                            unsigned int virtual_address) 
{
    unsigned int virt_page_num, i, LRU_page;
    access_result result;

    virt_page_num = virtual_address >> config.page_size;

    result.virtual_page_number = virt_page_num;

    if (table[pid][virt_page_num].valid) {
        result.physical_page_number = table[pid][virt_page_num].phys_page;
        result.physical_address = get_physical_address(virtual_address,
                                                 result.physical_page_number,
                                                 config.page_size);
        frame_table[result.physical_page_number].RU = time++;
        result.page_fault = false;
        return result;
    }

    LRU_page = 0;
    for (i = 0; i < trans_info.phys_frames; i++) {
        if (frame_table[i].in_use == false) {
            frame_table[i].in_use = true;
            frame_table[i].RU = time++;
            frame_table[i].pid = pid;
            table[pid][virt_page_num].phys_page = i;
            table[pid][virt_page_num].valid = true;
            result.physical_page_number = i;
            result.physical_address = get_physical_address(virtual_address,
                                                           i,
                                                           config.page_size);
            result.page_fault = true;
            return result;
        }
        if ((frame_table[i].RU < frame_table[LRU_page].RU
                && frame_table[i].pid == pid) || i == 0)
            LRU_page = i;
    }
    
    frame_table[LRU_page].RU = time++;
    frame_table[LRU_page].pid = pid;
    table[pid][virt_page_num].phys_page = LRU_page;
    table[pid][virt_page_num].valid = true;
    result.physical_page_number = LRU_page;
    result.physical_address = get_physical_address(virtual_address,
                                                   LRU_page,
                                                   config.page_size);
    result.page_fault = true;
    return result;
}

