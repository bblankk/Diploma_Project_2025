//header guard - only include this header once during compilation
#ifndef MEMAP_H //if 
#define MEMAP_H

#include <stdint.h>
#include "multiboot.h" //multiboot structs, yikes!


typedef struct
{
    uint64_t base; //physical starting address of this region (LOWEST physical address of the region)
    uint64_t length; // how long the region is (in bytes)
    uint32_t type;   // 1 = usable, others = reserved. Type guide at the start of memap.h
} memap_Region;

//region types: 
// 1: Available
// 2: Reserved
// 3: ACPI Reclaimable
// 4: ACPI NVS
// 5: Bad Memory

int32_t parse_Map(multibootInfo_t* mbInfo, memap_Region regions[], int32_t max_regions);
int32_t memap_Normalize( multibootInfo_t* mbInfo, memap_Region regions[], int32_t max_regions);

#endif //stop guard

