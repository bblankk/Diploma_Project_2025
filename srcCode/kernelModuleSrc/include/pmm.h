//functions for PHYSICAL MEMORY ALLOCATOR , CALLED HERE PMM (PHYSICAL MEMORY MANAGER)
#ifndef PMM_H
#define PMM_H

#include <stddef.h>
#include <stdint.h>
#include "memap.h"

// we need : initialize map, allocate, free;
// later: functions that work with vmm

/* initialize the physical memory manager
   regions       = normalized usable memory regions
   regionCount   = number of entries in regions[]
   bitmapBase    = physical address where the bitmap will live
*/
void pmm_Init(memap_Region regions[], uint32_t regionCount, uint64_t bitmapBase);

/* allocate one physical page (4KB)
   returns physical address of the page
   returns NULL if no free pages exist
*/
void* pmm_Alloc_Page(void);

/* free a previously allocated physical page
   addr must be page-aligned physical address
*/
void pmm_Free_Page(void* addr);




#endif 

