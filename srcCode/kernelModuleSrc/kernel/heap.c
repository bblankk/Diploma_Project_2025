#include "../include/heap.h"
#include "../include/vmm.h"

//heap grows upwards, towards high addresses

#define HEAP_START 0x400000   // right after the identity map (4MB)
#define HEAP_SIZE  0x100000   // 1MB heap


static uint32_t heapBase = 0;
static uint32_t heapTop = 0;
static uint32_t heapCurrent = 0;

void heap_Init(uint32_t heap_Base, uint32_t heap_Size)
{
    heapBase = heap_Base;
    heapTop = heap_Base + heap_Size;
    heapCurrent = heap_Base;

    // map heap pages
    for(uint32_t addr = heapBase; addr < heapTop; addr += 4096)
    {
        vmm_Map_Page(addr, addr, 0); // identity map for now
    }
}

void* kmalloc(uint32_t size)
{
    if (heapCurrent + size >= heapTop) //check if we got memory for dat
        return 0; // out of memory
    
    void* addr = (void*)heapCurrent;
    heapCurrent += size;
    
    return addr;
}