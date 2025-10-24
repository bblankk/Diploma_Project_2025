//declarations only for terminal functions.
#ifndef TERMINAL_H
#define TERMINAL_H

#include <stddef.h>
#include <stdint.h>

void terminal_initialize(void);
void terminal_printchar(char c);
void terminal_write(const char* data);

#endif
