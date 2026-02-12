#include <stdint.h>
#include "terminal.h"
#include "multiboot.h"
#include "../include/memap.h"




#define PAGE_SIZE 4096 // bytes per page table

// parses memory map and fills regions array (used for vmm/pmm)
int32_t parse_Map(multibootInfo_t* mbInfo, memap_Region regions[],  int32_t max_regions) 
{
    //if no regions
    if (!(mbInfo->flags & (1 << 6))) {
       return 0;
    }

    uint32_t mmapAddr   = mbInfo->mmapAddr;
    uint32_t mmapLength = mbInfo->mmapLength;

    int32_t regionCount = 0;
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


// page-aligns the base and end address of all regions in the regions[] array. Returns the amount of page-aligned type1 regions in total.
int32_t memap_Normalize( multibootInfo_t* mbInfo, memap_Region regions[], int32_t max_regions) 
{
    int32_t count = 0; //how many usable, normalized regions we've got ready to go!
    uint32_t offset = 0;
    uint64_t alignedBase = 0;
    uint64_t alignedEnd = 0;

 while (offset < mbInfo->mmapLength && count < max_regions) 
 {
    uint32_t mmapAddr   = mbInfo->mmapAddr;
    multibootMmapEntry_t* entry = (multibootMmapEntry_t*)(mmapAddr + offset);

    if (entry->type == 1) { //if the entry type is 1 (usable)

        uint64_t base = entry->addr;
        uint64_t end  = entry->addr + entry->len;

        // this if-else can become one longer but more complicated formula. This is more readable, though.
        if(base % PAGE_SIZE != 0) // if base is not page-aligned,
        {
            alignedBase = base + (PAGE_SIZE - (base % PAGE_SIZE)); //throw away the partial page at the beginning
        }
        else{  //and if it is page-aligned, leave it alone
            alignedBase = base;
        }
        alignedEnd = end - (end % PAGE_SIZE); //throw away the partial page at the end

            if (alignedEnd > alignedBase) {
                regions[count].base  = alignedBase;
                regions[count].length = alignedEnd - alignedBase;
                regions[count].type   = 1;
                count++;
            }
        }

        offset += entry->size + sizeof(entry->size); //multibood compliant construction, don't ask.
    }

    return count;
}


