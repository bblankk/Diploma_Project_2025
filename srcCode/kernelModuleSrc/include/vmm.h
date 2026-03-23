#ifndef VMM_H
#define VMM_H

#include <stdint.h>

//flags 
#define PAGE_PRESENT 0x1 //page exists
#define PAGE_WRITE   0x2 //page can be overwritten 

void vmm_Init();
void vmm_MapPage(uint32_t virtualAddr, uint32_t physicalAddr, uint32_t flags);

#endif