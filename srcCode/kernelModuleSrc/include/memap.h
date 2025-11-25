//header guard - only include this header once during compilation
#ifndef MEMAP_H //if 
#define MEMAP_H

#include <stdint.h>
#include "multiboot.h" //struct def

void print_Memory_Map(multibootInfo_t* mbInfo);

#endif //stop guard

