//interrupt service routines
//only shared interrupt definitions

// a snapshot of the CPU state at the time of an interrupt. Mostly used for flagging and such.
typedef struct interrupt_Context
{
    // saves registers
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    // saves... well. heh.
    uint64_t interruptNumber;
    uint64_t errorCode;
    // stack location, interrupt number, flags etc. 
    uint64_t instructionPointer;
    uint64_t codeSegment;
    uint64_t flags;
    uint64_t stackPointer;
    uint64_t stackSegment;

} interrupt_Context;


typedef void (*InterruptHandler)(CPURegisters *);

void register_Interrupt_Handler( uint8_t interruptNumber, InterruptHandler handler );

void global_Interrupt_Delegator( CPURegisters *registers );
