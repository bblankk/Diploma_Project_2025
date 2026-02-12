//functions for PHYSICAL MEMORY ALLOCATOR , CALLED HERE PMM (PHYSICAL MEMORY MANAGER)
#ifndef PMM_H
#define PMM_H

#include <stddef.h>
#include <stdint.h>


// we need : initialize map, allocate, free;
// later: functions that work with vmm

void  pmm_Init(memmap_t* map);
void* pmm_Alloc_Page(void);
void  pmm_Free_Page(void* phys);
size_t pmm_Total_Pages(void);


#endif 

