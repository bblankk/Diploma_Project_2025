#pragma once
#include <stdint.h>

void bitmap_set(uint64_t index);    // mark used
void bitmap_clear(uint64_t index);  // mark free
uint8_t bitmap_test(uint64_t index); // 1 = used, 0 = free

void bitmap_Init(uint64_t bitmapBase, uint64_t totalPages);
