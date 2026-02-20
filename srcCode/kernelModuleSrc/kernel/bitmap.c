/* These functions are used in the kernel , sequentially AFTER memap initialization, which they rely on.

1 page = 4096 bytes (real memory)
1 page = 1 bit (inside bitmap tracking) this is 1 PAGE inside the bitmap.  yes there are two incredibly, fundamentally, different meanings for the word 'page'. Yes they will be used interchangeably. Stay on your toes.
1 bitmap byte = tracks 8 pages . yes you will have to track every bit in that byte. don't look at me like that, i didn't make the rules.

example w 8 pages
real mem:
[4KB][4KB][4KB][4KB][4KB][4KB][4KB][4KB]

Bitmap (1 byte = 8 bits):
[1 0 0 1 1 0 0 0]

in conclusion: 

one bit = one physical page
index = physAddr / 4096
bitmap stored at bitmapBase
PMM scans bitmap for free pages
*/

#include <stdint.h>
#include "../include/bitmap.h"
#include "../include/memap.h"

// !! ------------------ FUNCTIONS FOR INITIALIZATION AND INITIAL CLEARS. used by kernel.


#define PAGE_SIZE 4096

static uint64_t totalPages = 0;
static uint8_t* bitmap = 0;




//this function initiates the bitmap with the total amount of pages and marks all pages as used (1) to be later marked as free (0) when they are found in the safe-to-use regions. Used in initialization on startup.
void bitmap_Init(uint64_t pages, uint64_t bitmapBase)
{
    totalPages = pages;
    bitmap = (uint8_t*)bitmapBase;

    uint64_t bytes = (pages + 7) / 8;

    for (uint64_t i = 0; i < bytes; i++)
        bitmap[i] = 0xFF;   // mark all used
}




// frees all usable pages, aka flags them 0 on the internal bookkeeping. This is for initialization on startup.
void bitmap_InitFreePages(memap_Region regions[], uint32_t regionCount)
{
    for(uint32_t i=0;i<regionCount;i++)
    {
        if(regions[i].type != 1)
            continue;

        uint64_t start = regions[i].base;
        uint64_t end   = start + regions[i].length;

        for(uint64_t addr = start; addr < end; addr += PAGE_SIZE)
        {
            uint64_t index = addr / PAGE_SIZE;
            bitmap_clear(index);
        }
    }
}






// !! ------------------------------- FUNCTIONS FOR ALLOCATING/DEALLOCATING/TESTING BITS. Used by PMM.
//used later.
void bitmap_set(uint64_t index)
{
    bitmap[index / 8] |= (uint8_t)(1u << (index % 8));
}

void bitmap_clear(uint64_t index)
{
    bitmap[index / 8] &= (uint8_t)~(1u << (index % 8));
}

uint8_t bitmap_test(uint64_t index)
{
    return (bitmap[index / 8] >> (index % 8)) & 1u;
}



// optional getter so PMM can know limits
uint64_t bitmap_TotalPages(void)
{
    return totalPages;
}