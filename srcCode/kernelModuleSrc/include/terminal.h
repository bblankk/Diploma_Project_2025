//declarations only for terminal functions.
#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

void terminal_Initialize(void);
void terminal_Printchar(char c) ;
void terminal_Write(const char* data);
void terminal_PrintHex32(uint32_t num) ;
void terminal_PrintHex64(uint64_t num);



#endif
