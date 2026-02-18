// The Interrupt Service Routine (ISR) is the code executed to handle hardware/software interrupts.

#include "terminal.h"

void isr0_handler(void)
{
    terminal_Write("Divide by zero exception!\n");
    for (;;);
}