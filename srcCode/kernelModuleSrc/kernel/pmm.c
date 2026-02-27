//PHYSICAL MEMORY MANAGER (ALLOCATOR), shortened to PMM
#include <stddef.h>
#include <stdint.h>
#include "../include/pmm.h"
#include "../include/bitmap.h"

//bitmap isnt here, its in bitmap.c. PMM only manipulates it, but it's not here.
static uint32_t totalPages = 0;
#define PAGE_SIZE 4096

 /* param regions is a pointer to the start of the regions array
 param regionCount is an unsigned integer for the total amount of expected safe-to-use regions
 bitmap base is the phys address the bitmap starts. */
void pmm_Init(memap_Region regions[], uint32_t regionCount, uint32_t bitmapBase)
{
    uint32_t physTop = 0;

    //finding highest physical address of the memap
    for(uint32_t i=0;i<regionCount;i++)
    {
        uint32_t end = (uint32_t)(regions[i].base + regions[i].length);
        if(end > physTop)
            physTop = end;
    }

    //  page align 
    physTop = (physTop + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
    totalPages = physTop / PAGE_SIZE;

    bitmap_Init(totalPages, bitmapBase); // initialize bitmap (all pages used)
    bitmap_InitFreePages(regions, regionCount); //free usable pages
    
    // reserve kernel pages.
    extern uint8_t kernelEnd; // from linker.ld - pointer to kernel's end
    uint32_t kernelEndAddr = (uint32_t)&kernelEnd;  // converts pointer to an address
    uint32_t kernelPages =(kernelEndAddr + PAGE_SIZE - 1) / PAGE_SIZE; //calculates how many pages the kernel occupies
    for(uint32_t i = 0; i < kernelPages; i++)
    {
         bitmap_set(i); // mark all kernel pages as used, so the PMM may not use them.
    }
       

}

// frees all usable pages, aka flags them 0 on the internal bookkeeping. This is for initialization on startup. used in pmm_Init.
void bitmap_InitFreePages(memap_Region regions[], uint32_t regionCount)
{
    for(uint32_t i=0;i<regionCount;i++)
    {
        if(regions[i].type != 1)
            continue;

        uint32_t start = (uint32_t)regions[i].base;
        uint32_t end   = (uint32_t)(regions[i].base + regions[i].length);

        for(uint32_t addr = start; addr < end; addr += PAGE_SIZE)
        {
            uint32_t index = addr / PAGE_SIZE;
            bitmap_clear(index);
        }
    }
}

void* pmm_Alloc_Page(void)
{
    for(uint32_t i=0;i<totalPages;i++)
    {
        if(!bitmap_test(i))
        {
            bitmap_set(i);
            return (void*)(i*PAGE_SIZE);
        }
    }

    return NULL;
}

void pmm_Free_Page(void* addr)
{
    uint32_t index = ((uint32_t)addr)/PAGE_SIZE;
    bitmap_clear(index);
}

uint32_t pmm_Total_Pages(void)
{
     return totalPages; 
}



//define rom (read only memory) and dont touch it
// read only memory is a small block of code on the motherboard in some chip you might recognize as the BIOS chip, and it's commonly known for storing BIOS and other untouchable firmware. What's on there is decided by the manufacturer and NONE of our business, we won't be touching it.

// global memory  (.bss stuff) - a chunk that's allocated at program startup and not changed

//lexically allocated memory - memory which is automatically allocated in a block when a function begins and deallocated when it returns. It can be seen as an area of 'scratch memory' which the program can use for temporary values whose lifespan is determined by the running time of the function which is using it, and can always be automatically reclaimed for reuse after the function exits.

//dynamically allocated memory - hardest part here, beware memory holes and implement all the thread management stuff C will need later on.

// side quest - garbage collector  ( GC strat )
