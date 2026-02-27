#ifndef INTERRUPT_DELEGATOR_H
#define INTERRUPT_DELEGATOR_H
#include <stdint.h>


// a snapshot of the CPU state at the time of an interrupt. Well, a snapshot of the stack , lol. Those registers get pushed IN THAT ORDER in IDT.asm before any actual handlers get called. Save first, edit later! haha. Mostly used for flagging and such. and restoring state.
typedef struct interrupt_Context {

    // stack snapshot
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;

    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // I really think the names are self-explanatory. Do you WANT to read more comments?
    uint32_t interruptNumber;
    uint32_t errorCode;

} interrupt_Context;

// easier to read later on so.. interruptHandler is a ptr to a function that returns void, and takes param interrupt_Context
typedef void (*InterruptHandler)(interrupt_Context *context);
// fills the interrupt handler table
void registerInterruptHandler(uint8_t interruptNumber, InterruptHandler handler);
//interrupt delegator function; main functionality
// function takes 1 param type interrupt_Context and returns nothing.
void globalInterruptDelegator(interrupt_Context *context);

#endif