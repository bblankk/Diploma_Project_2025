#ifndef VMM_H
#define VMM_H

#include <stdint.h>

void vmm_Init(void);
void vmm_Map_Page(uint32_t virtualAddr, uint32_t physicalAddr, uint32_t flags);

#endif