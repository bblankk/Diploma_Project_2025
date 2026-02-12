
/* These functions are used in the kernel , sequentially AFTER memap initialization, which they rely on.

1 page = 4096 bytes (real memory)
1 page = 1 bit (inside bitmap tracking) this is 1 PAGE inside the bitmap.  yes there are two incredibly, fundamentally, different meanings for the word 'page'. Yes they will be used interchangeably. Stay on your toes.
1 bitmap byte = tracks 8 pages . yes you will have to track every bit in that byte. don't look at me like that, i didn't make the rules.

example w 8 pages
real mem:
[4KB][4KB][4KB][4KB][4KB][4KB][4KB][4KB]

Bitmap (1 byte = 8 bits):
[1 0 0 1 1 0 0 0]

*/

#include <stdint.h>
#include "../include/bitmap.h"

// !! ------------------ FUNCTIONS FOR INITIALIZATION AND INITIAL CLEARS. used by kernel.


#define PAGE_SIZE 4096 // a real memory page is 4kb, 4096 bytes.
static uint64_t pmmTotalPages = 0; // this will be used internally.
static uint8_t* bitmap = 0;

static uint64_t physBase = 0;
static uint64_t physTop  = 0;

/* this function calculates the total pages of the final bitmap
 param regions is a pointer to the start of the regions array
 param regionCount is an unsigned integer for the total amount of expected safe-to-use regions */
static void compute_PhysBounds(memap_Region regions[], uint32_t count)
{
    physBase = (uint64_t)-1;
    physTop  = 0;

    for (uint32_t i = 0; i < count; i++)
    {
        if (regions[i].base < physBase)
            physBase = regions[i].base;

        uint64_t end = regions[i].base + regions[i].length;
        if (end > physTop)
            physTop = end;
    }

    physBase &= ~(PAGE_SIZE - 1);
    physTop   = (physTop + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

    pmmTotalPages = (physTop - physBase) / PAGE_SIZE;
}





//this function initiates the bitmap with the total amount of pages and marks all pages as used (1) to be later marked as free (0) when they are found in the safe-to-use regions. Used in initialization on startup.
void bitmap_Init(memap_Region regions[], uint32_t regionCount, uint64_t bitmapBase)
{
    compute_PhysBounds(regions, regionCount);

    uint64_t bitmapBytes = (pmmTotalPages + 7) / 8;

    bitmap = (uint8_t*)bitmapBase;

    for (uint64_t i = 0; i < bitmapBytes; i++)
        bitmap[i] = 0xFF;
}




// frees all usable pages, aka flags them 0 on the internal bookkeeping. This is for initialization on startup.
void bitmap_InitFreePages(memap_Region regions[], uint32_t regionCount)
{
    for(uint32_t i =0 ; i< regionCount; i++)
    {
        if(regions[i].type == 1) // if the region is usable
        {
            uint64_t start = regions[i].base;
            uint64_t end   = regions[i].base + regions[i].length;
            for (uint64_t addr = start; addr < end; addr += PAGE_SIZE) // loops every real mem page in the region. 
                {
                    uint64_t page_index = (addr - physBase) / PAGE_SIZE;
                    uint64_t byte = page_index / 8;
                    uint8_t  bit  = page_index % 8;

                    bitmap[byte] &= (uint8_t)~(1u << bit); // more shenanighans. long story short, only the current bit(page flag) gets freed. long story below.
                }
        }
    }
}
/* 
explanation for line : bitmap[byte] &= ~(1 << bit)
bitmap[byte] = 0b11111111   // all pages used
bit = 3                       // page we want to free
1 << 3    = 0b00001000
~(1<<3)   = 0b11110111       // mask: flip that bit
AND with bitmap:
0b11111111 & 0b11110111 = 0b11110111 
*/






// !! ------------------------------- FUNCTIONS FOR ALLOCATING/DEALLOCATING/TESTING BITS. Used by PMM & VMM.
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
uint64_t bitmap_total_pages(void)
{
    return pmmTotalPages;
}
