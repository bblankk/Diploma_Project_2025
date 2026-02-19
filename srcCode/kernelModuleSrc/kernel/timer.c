#include <stdint.h>
#include "terminal.h"
#include "pic.h"

static uint32_t tick = 0;

void irq0_Handler(void)
{
    tick++;
    
    if (tick % 100 == 0)
        terminal_Write("Tick\n"); //writes to terminal. this will later not be shown, stays for debug purposes.

    pic_SendEOI(0); //end of exception! :D
}
