#ifndef VMM_H
#define VMM_H

#include <stdint.h>

void vmm_Init(void);
void vmm_MapPage(uint32_t virtualAddr, uint32_t physicalAddr);

#endif