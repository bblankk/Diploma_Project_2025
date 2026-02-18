#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// global descriptor table initialization. once during compile time.
void gdt_Init(void);

#endif
