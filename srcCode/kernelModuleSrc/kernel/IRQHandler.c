#include <stdint.h>

#include "IRQHandler.h"
#include "interruptDelegator.h"
#include "terminal.h"

//this is the actual interrupts logic. After many layers, this is the base level where the actual interrupt logic is defined. Yes, you've reached the bottom of the barrel. There's no more.

static uint32_t timerTicks = 0;

//interrupt request 0 - the reclusive timer! muhaha
// timer handler, irq number 32
void irq0_Handler(interrupt_Context *context) {

    timerTicks++;
    terminal_Write("tick");
    //no pic calls, delegator handles that :)

}

// divide by 0 handler, irq number i dunno
void isr0_Handler(interrupt_Context *context) {

    terminal_Write("Divide by zero exception\n");

    for(;;) { }

}