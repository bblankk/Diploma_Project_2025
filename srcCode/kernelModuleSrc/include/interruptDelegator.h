#ifndef INTERRUPT_DELEGATOR_H
#define INTERRUPT_DELEGATOR_H
#include <stdint.h>


// a snapshot of the CPU state at the time of an interrupt. Well, a snapshot of the stack , lol. Those registers get pushed IN THAT ORDER in IDT.asm before any actual handlers get called. Save first, edit later! haha. Mostly used for flagging and such. and restoring state.
typedef struct interrupt_Context {
    //registers
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t esp;

    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;

    // interrupt number (used for gates) and error code.
    uint32_t interruptNumber;
    uint32_t errorCode;

    //nonsense really. I haven't used them yet and probably won't till debugging demands it. Would look cool on a chart though.
    uint32_t instructionPointer;
    uint32_t codeSegment;
    uint32_t flags;
    uint32_t stackPointer;
    uint32_t stackSegment;

} interrupt_Context;

//interrupt delegator function; main functionality
// function takes 1 param type interrupt_Context and returns nothing.
void globalInterruptDelegator(interrupt_Context *context);

#endif