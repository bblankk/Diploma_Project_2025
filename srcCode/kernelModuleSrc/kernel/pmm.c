//PHYSICAL MEMORY MANAGER (ALLOCATOR), shortened to PMM
#include <stddef.h>
#include <stdint.h>
#include "../include/pmm.h"
#include "../include/bitmap.h"

//bitmap isnt here, its in bitmap.c. PMM only manipulates it, but it's not here.
static uint64_t totalPages = 0;


 /* param regions is a pointer to the start of the regions array
 param regionCount is an unsigned integer for the total amount of expected safe-to-use regions */
void pmm_Init(memap_Region regions[], uint32_t regionCount, uint64_t bitmapBase) {
    // Compute lowest/highest physical addresses
    uint64_t physBase = UINT64_MAX;
    uint64_t physTop = 0;
    for (int i = 0; i < regionCount; i++) {
        if (regions[i].base < physBase) physBase = regions[i].base;
        uint64_t end = regions[i].base + regions[i].length;
        if (end > physTop) physTop = end;
    }
    physBase &= ~(4096-1);
    physTop   = (physTop + 4095) & ~(4096-1);

    totalPages = (physTop - physBase)/4096;

    bitmap_init(bitmapBase, totalPages);
    bitmap_init_free_pages(regions, regionCount);
}

void* pmm_Alloc_Page(void) {
    for (uint64_t i = 0; i < totalPages; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            return (void*)(bitmap_phys_base() + i*4096);
        }
    }
    return NULL; // no mooore memoryyy
}

void pmm_Free_Page(void* phys) {
    uint64_t idx = ((uint64_t)phys - bitmap_phys_base()) / 4096;
    bitmap_clear(idx);
}

uint64_t pmm_Total_Pages(void) { return totalPages; }



//define rom (read only memory) and dont touch it
// read only memory is a small block of code on the motherboard in some chip you might recognize as the BIOS chip, and it's commonly known for storing BIOS and other untouchable firmware. What's on there is decided by the manufacturer and NONE of our business, we won't be touching it.

// global memory  (.bss stuff) - a chunk that's allocated at program startup and not changed

//lexically allocated memory - memory which is automatically allocated in a block when a function begins and deallocated when it returns. It can be seen as an area of 'scratch memory' which the program can use for temporary values whose lifespan is determined by the running time of the function which is using it, and can always be automatically reclaimed for reuse after the function exits.

//dynamically allocated memory - hardest part here, beware memory holes and implement all the thread management stuff C will need later on.

// side quest - garbage collector  ( GC strat )
