#ifndef BITMAP_H
#define BITMAP_H

#include <stdint.h>

//inits
void bitmap_Init(uint32_t pages, uint32_t bitmapBase);


//usables
void bitmap_set(uint32_t index);
void bitmap_clear(uint32_t index);
uint8_t bitmap_test(uint32_t index);
uint32_t bitmap_TotalPages(void);

#endif
