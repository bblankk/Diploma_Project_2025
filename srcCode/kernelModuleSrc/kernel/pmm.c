//PHYSICAL MEMORY ALLOCATOR, ALSO CALLED PMM (PHYSICAL MEMORY MANAGER) - MAIN FUNCTIONALITY LIVES HERE <22

#include <stddef.h>
#include <stdint.h>
//#include "../include/pmm.h"
#include "../include/memap.h"
#include "../include/pmm.h"

//second: a small contiguous pool tracked by a bitmap
//these will be used for the bitmap

//bitmap steps; decide the physical address range - lowest to highest for all regions
// compute total page count for every region
// allocate bitmap storage
// initialize all bits 
// walk usable regions to check if everythin workin ??
// functions for allocating / deallocating  pages

void pmm_Init(memap_Region regions[], int32_t regionCount, uint64_t bitmapPhysicalBase ) {
    

}


//define rom (read only memory) and dont touch it
// read only memory is a small block of code on the motherboard in some chip you might recognize as the BIOS chip, and it's commonly known for storing BIOS and other untouchable firmware. What's on there is decided by the manufacturer and NONE of our business, we won't be touching it.

// global memory  (.bss stuff) - a chunk that's allocated at program startup and not changed

//lexically allocated memory - memory which is automatically allocated in a block when a function begins and deallocated when it returns. It can be seen as an area of 'scratch memory' which the program can use for temporary values whose lifespan is determined by the running time of the function which is using it, and can always be automatically reclaimed for reuse after the function exits.

//dynamically allocated memory - hardest part here, beware memory holes and implement all the thread management stuff C will need later on.

// side quest - garbage collector  ( GC strat )
