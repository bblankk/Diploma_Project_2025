// The Interrupt Service Routine (ISR) is the code executed to handle hardware/software interrupts. It's been renamed to interruptDelegator... cause that's what it does... and i hate OS naming conventions. Let this be my strike on them.
// Main responsibilities is to !ROUTE INTERRUPTS! to their handlers (the function that actually does something about it).
// this is the cool manager layer, basically. you go here, you go there!
#include "interruptDelegator.h"
#include "IRQHandler.h"
#include "pic.h"

static InterruptHandler interruptHandlerTable[256];


void registerInterruptHandler(uint8_t interruptNumber, InterruptHandler handler) 
{
    interruptHandlerTable[interruptNumber] = handler;

}

void globalInterruptDelegator(interrupt_Context *context)
{

    InterruptHandler handler = interruptHandlerTable[context->interruptNumber];

    if(handler) {

        handler(context);

    }

    if(context->interruptNumber >= 32 &&
       context->interruptNumber <= 47) {

        pic_SendEOI(
            context->interruptNumber - 32
        );

    }

}