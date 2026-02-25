// The Interrupt Service Routine (ISR) is the code executed to handle hardware/software interrupts. It's been renamed to interruptDelegator... cause that's what it does... and i hate OS naming conventions. Let this be my strike on them.
// Main responsibilities is to route interrupts to their handlers (the function that actually does something about it).
// this is the cool manager layer, basically. you go here, you go there!
#include "interruptDelegator.h"
#include "pic.h"

InterruptHandler interruptTable[256];


void register_interrupt_handler(  uint8_t interrupt_number, InterruptHandler handler)
{
    interruptTable[interrupt_number] = handler;
}


void global_interrupt_delegator( interrupt_Context *context)
{
    InterruptHandler handler =
        interruptTable[context->interrupt_number];

    if(handler) handler(context);

    /* Send EOI for hardware IRQs */
    if(context->interrupt_number >= 32 &&
       context->interrupt_number <= 47)
    {
        pic_SendEOI( context->interrupt_number - 32);
    }
}