#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>

void* kmalloc(uint32_t size);
void heap_Init(uint32_t heap_Base, uint32_t heap_Size);

#endif
