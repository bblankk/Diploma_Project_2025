/*Kernel subsystems (heap, paging setup, drivers) request virtual memory ? VMM maps pages ? VMM asks PMM for physical frames.
vmm is basically a layer of needed abstraction on top of the physical memory.

chain of a memory request:
CPU -> virtual -> pageDirectory -> pageTable -> PMM (operates the bitmap, internal bookeeping structures)
PMM manipulates the bitmap derived from the memmap. The bitmap and memmap are implementation details hidden inside PMM. */

#include "../include/vmm.h"
#include "../include/pmm.h"

#define PAGE_PRESENT 0x1
#define PAGE_WRITE   0x2

uint32_t* pageDirectory = 0; /*array of  1024 entries. or behaves like one, because it points to a contiguous memory strtucture ( a page ) 

(aka 1024 entries, 4bytes per entry = 32 bits per entry, top 20bits is the address (split into top 10 PD bottom 10 PT) and bottom  12bits holds the flags  )
ONE entry: 
| 10 bits | 10 bits | 12 bits                       |
|   PD    |   PT    | offset (position inside page) |
example entry: pageTable[i] = physicalAddr | PAGE_PRESENT | PAGE_WRITE; */



//allocates one physical page to store vmm's own directory
void vmm_Init(void) {
    pageDirectory = (uint32_t*)pmm_Alloc_Page(); // now points to one full 4kb page :) 

    //clear pd
    for(int i = 0; i < 1024; i++)
    {
        pageDirectory[i] = 0;
    }
    //identity map first 4mb, a small safe buffer for when paging turns on
for(uint32_t i = 0; i < 4 * 1024 * 1024; i += 4096)
{
    vmm_Map_Page(i, i, PAGE_WRITE); //flag = page writable
}

// loading page directory 
asm volatile("mov %0, %%cr3" :: "r"(pageDirectory));

// enables paging
 uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}



//core functionality - when CPU sees virtual page x, go to physical page y
  void vmm_Map_Page(uint32_t virtualAddr, uint32_t physicalAddr, uint32_t flags)
{
    uint32_t pageDirectoryIndex = virtualAddr >> 22;  //highest 10 bits
    uint32_t pageTableIndex = (virtualAddr >> 12) & 0x03FF;  //removes offset , keeps only mid 10 bits

    
    uint32_t* pageTable; 

    if (!(pageDirectory[pageDirectoryIndex] & PAGE_PRESENT)) // no table yet , so create one
    {
        pageTable = (uint32_t*)pmm_Alloc_Page();

        for(int i = 0; i < 1024; i++) //init table
            pageTable[i] = 0;

        pageDirectory[pageDirectoryIndex] = ((uint32_t)pageTable) | flags | PAGE_PRESENT; //combine and link
    }
    else
    {
        pageTable = (uint32_t*)(pageDirectory[pageDirectoryIndex] & 0xFFFFF000); //mask removes flags and gives real address 
    }

    pageTable[pageTableIndex] = (physicalAddr & 0xFFFFF000) | flags | PAGE_PRESENT; //final mapping , one entry 
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
pageTableIndex = (0x12345678 >> 12) & 0x3FF;
Result:
0x345 (decimal 837)
-> This selects entry 837 inside that page table

Offset: 
offset = 0x12345678 & 0xFFF;
Result:
0x678
-> This is the byte inside the 4KB page
*/