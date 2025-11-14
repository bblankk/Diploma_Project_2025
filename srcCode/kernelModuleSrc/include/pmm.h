//functions for PHYSICAL MEMORY ALLOCATOR , CALLED HERE PMM (PHYSICAL MEMORY MANAGER)
#ifndef PHYSICAL_MEMORY_ALLOCATOR_H
#define PHYSICAL_MEMORY_ALLOCATOR_H
#include <stddef.h>
#include <stdint.h>

void initialize_Physical_Memory_Allocator(uint32_t totalMemorySize, uint32_t kernelEndAddress);
void* allocate_Physical_Memory(size_t size);
void free_Physical_Memory(void* address, size_t size);
#endif 

