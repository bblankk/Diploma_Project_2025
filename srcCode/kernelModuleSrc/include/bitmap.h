#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>
#include "memap.h"

//inits
uint64_t count_Pages(memap_Region regions[], uint32_t regionCount);
void bitmap_Init(memap_Region regions[], uint32_t regionCount, uint64_t bitmapBase);
void bitmap_InitFreePages(memap_Region regions[], uint32_t regionCount);

//usables
void bitmap_set(uint64_t index);
void bitmap_clear(uint64_t index);
uint8_t bitmap_test(uint64_t index);


#endif
