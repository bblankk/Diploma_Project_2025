#include <stdint.h>
#include "terminal.h"
#include "multiboot.h"
#include "../include/memap.h"

const char* mem_Type_Name(uint32_t type) {
    switch (type) {
        case 1: return "Available";
        case 2: return "Reserved";
        case 3: return "ACPI Reclaimable";
        case 4: return "ACPI NVS";
        case 5: return "Bad Memory";
        default: return "Unknown";
    }
}


//memap_Region regions[32];
//this is some kind of initializing function actually
void print_Memory_Map(multibootInfo_t* mbInfo, memap_Region regions[],  size_t max_regions) {
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

