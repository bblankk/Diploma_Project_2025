#include <stdint.h>
#include "terminal.h"
#include "multiboot.h"
#include "../include/memap.h"

// const char* mem_Type_Name(uint32_t type) {
//     switch (type) {
//         case 1: return "Available";
//         case 2: return "Reserved";
//         case 3: return "ACPI Reclaimable";
//         case 4: return "ACPI NVS";
//         case 5: return "Bad Memory";
//         default: return "Unknown";
//     }
// }

#define PAGE_SIZE 4096

// parser , fills array (used for vmm/pmm)
void print_Memory_Map(multibootInfo_t* mbInfo, memap_Region regions[],  size_t max_regions) 
{
    //if no regions
    if (!(mbInfo->flags & (1 << 6))) {
       return 0;
    }

    uint32_t mmapAddr   = mbInfo->mmapAddr;
    uint32_t mmapLength = mbInfo->mmapLength;

    size_t regionCount = 0;
    for (uint32_t offset = 0; offset < mmapLength  && regionCount < max_regions;   ) {
        multibootMmapEntry_t* entry = (multibootMmapEntry_t*)(mmapAddr + offset);
        
        regions[regionCount].base = entry->addr; 
        regions[regionCount].length = entry->len;
        regions[regionCount].type = entry->type;

        regionCount++;
        offset += entry->size + sizeof(entry->size); 
    }
    return regionCount;
    
}

//normalize usable ranges (making them "clean, page-aligned ranges") so pmm/vmm have an easier time with bookeeping and paging
//clean up each range so the pages can be EXACT
// Example after normalization:
// Region A: start = 0x00100000, pages = 256
// Region B: start = 0x01000000, pages = 1024
// Region C: start = 0x08000000, pages = 8192

uint64_t alignedBase =  base + (PAGE_SIZE - (base % PAGE_SIZE)) % PAGE_SIZE; //throw away partial range at the start

uint64_t end = base + length;
uint64_t alignedEnd = end - (end % PAGE_SIZE); //throw away partial page at the end
 //normalizing function   ADD TO .H FILE LATER
size_t memap_Normalize( multibootInfo_t* mbInfo, memap_Region regions[], size_t max_regions) 
{
    size_t count = 0;
    uint32_t offset = 0;
 while (offset < mbInfo->mmapLength && count < max_regions) 
 {
    uint32_t mmapAddr   = mbInfo->mmapAddr;
    multibootMmapEntry_t* entry = (multibootMmapEntry_t*)(mmapAddr + offset);

    if (entry->type == 1) { 

        uint64_t base = entry->addr;
        uint64_t end  = entry->addr + entry->len;
        uint64_t alignedBase =base + (PAGE_SIZE - (base % PAGE_SIZE)) % PAGE_SIZE;
        uint64_t alignedEnd = end - (end % PAGE_SIZE);

            if (alignedEnd > alignedBase) {
                regions[count].base  = alignedBase;
                count++;
            }
        }

        offset += entry->size + sizeof(entry->size);
    }

    return count;
}


