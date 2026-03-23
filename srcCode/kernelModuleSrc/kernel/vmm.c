/*Kernel subsystems (heap, paging setup, drivers) request virtual memory ? VMM maps pages ? VMM asks PMM for physical frames.
vmm is basically a layer of needed abstraction on top of the physical memory.

chain of a memory request:
CPU -> virtual -> pageDirectory -> pageTable -> PMM (operates the bitmap, internal bookeeping structures)
PMM manipulates the bitmap derived from the memmap. The bitmap and memmap are implementation details hidden inside PMM. */

#include "../include/vmm.h"
#include "../include/pmm.h"

#define PAGE_SIZE 4096

uint32_t* pageDirectory; /*array of  1024 entries. or behaves like one, because it points to a contiguous memory strtucture ( a page )
(aka 1024 entries, 4bytes per entry = 32 bits per entry, lower 20bits is the address and top 12bits holds the flags  )
example entry: pageTable[i] = physicalAddr | PAGE_PRESENT | PAGE_WRITE; (phys address and flags) */


//allocates one physical page to store vmm's own directory
void vmm_Init() {
    pageDirectory = (uint32_t*)pmm_Alloc_Page(); // now points to one full 4kb page :) 

    for(int i = 0; i < 1024; i++)
    {
  pageDirectory[i] = 0;
    }
for(uint32_t i = 0; i < 4 * 1024 * 1024; i += 4096)
{
    vmm_MapPage(i, i, PAGE_WRITE);
}

loadPageDirectory(pageDirectory);
enablePaging();
}



//core functionality - when CPU sees virtual page x, go to physical page y
  void vmm_MapPage(uint32_t virtualAddr, uint32_t physicalAddr, uint32_t flags)
{
    uint32_t pageDirectoryIndex = virtualAddr >> 22; 
    uint32_t ptIndex = (virtualAddr >> 12) & 0x03FF;

    uint32_t* pageTable;

    if (!(pageDirectory[pageDirectoryIndex] & PAGE_PRESENT))
    {
        pageTable = (uint32_t*)pmm_Alloc_Page();

        for(int i = 0; i < 1024; i++)
            pageTable[i] = 0;

        pageDirectory[pageDirectoryIndex] = ((uint32_t)pageTable) | flags | PAGE_PRESENT;
    }
    else
    {
        pageTable = (uint32_t*)(pageDirectory[pageDirectoryIndex] & 0xFFFFF000);
    }

    pageTable[ptIndex] = (physicalAddr & 0xFFFFF000) | flags | PAGE_PRESENT;
}


//for enabling paging
static inline void loadPageDirectory(uint32_t* pd)
{
    asm volatile("mov %0, %%cr3" :: "r"(pd));
}

static inline void enablePaging()
{
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}



//address conversion virtual to physical

/*
virtual address : 0x12345678

Binary:
00010010001101000101011001111000

Split into parts:
 Directory (10 bits) , Table (10 bits), Offset (12 bits)

*/
/*
Now compute each part.

pageDirectoryIndex = 0x12345678 >> 22;
Result:
0x48 (decimal 72)
-> This selects entry 72 in pageDirectory


PageTableIndex:
ptIndex = (0x12345678 >> 12) & 0x3FF;
Result:
0x345 (decimal 837)
-> This selects entry 837 inside that page table

Offset: 
offset = 0x12345678 & 0xFFF;
Result:
0x678
-> This is the byte inside the 4KB page
*/