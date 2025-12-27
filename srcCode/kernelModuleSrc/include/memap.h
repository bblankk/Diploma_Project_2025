//header guard - only include this header once during compilation
#ifndef MEMAP_H //if 
#define MEMAP_H

#include <stdint.h>
#include "multiboot.h" //struct def
#include "pmm.h"


typedef struct
{
    uint64_t base; //physical starting address of this region (LOWEST physical address of the region)
    uint64_t length; // how long the region is (in bytes)
    uint32_t type;   // 1 = usable, others = reserved
} memap_Region;

size_t print_Memory_Map(multibootInfo_t* mbInfo, memap_Region regions[], size_t max_regions);

#endif //stop guard

