// we're currently in protected real mode 32bit
//full access to address space :3
// apparently a stack pointer has been helpfully set by GRUB already. You can use stack - related C functions.
//currently in a nakey environment - just cpu , no libc, no bios calls.
// linker.ld is the roadmap for the kernel elf (which includes all files in the current working structure, btw)! 

#include <stddef.h>
#include <stdint.h> // implements fixed-width integer types int8_t, uint16_t, int32_t, and uint64_t (among others). 
 #include "terminal.h"  // terminal functionality
#include "multiboot.h"  // multiboot protocol. needed for memory map.
#include "memap.h" // memory map functionality , aka bitmap shenanighans.
#include "pic.h"
#include "pmm.h"
#include "gdt.h"
#include "idt.h"
#include "interruptDelegator.h"
#include "IRQHandler.h"

// Note that linker symbols are not variables, they have no memory allocated for maintaining a value, rather their address is their value.
extern uint8_t kernelEnd; // from linker.ld , this is a pointer to the ADDRESS Of the end of the kernel


//kmain expects the ebx pointer (for the memory map) to be first argument on the stack. (pushed last)
void kmain(multibootInfo_t* mbInfo) {

    asm volatile("cli"); // disable interrupts until IDT (interrupt descriptor table) is ready


    terminal_Initialize();
    terminal_Write("Terminal initialized\n");

    terminal_Write("Before PMM\n");
    //initializing pmm and vmm's initial memory readings
    memap_Region regions[32]; //parse, normalize, consume 
    int32_t regionCount; // the amount of available and normalized regions, ready to be used
    regionCount = parse_Map(mbInfo, regions, 32); //parse + fill regions[32]. Returns the amount of normalized type1 regions parsed.
    // todo print regins count
    regionCount = memap_Normalize(mbInfo, regions, 32); // page-align region base and length
    // todo print region count again to check its the same
    uint32_t bitmapPhysicalBaseAddress = (uint32_t)&kernelEnd; // now this is the numerical value for the physical address where pmm's bitmap will live. Linker script gives it to us.
    pmm_Init(regions, regionCount, bitmapPhysicalBaseAddress);
    terminal_Write("PMM initialized.\n Before GDT \n");

    //global descriptor table for interrupts.
    gdt_Init(); //global descriptor table - defines code, data, and what it means to be lost and chase hopes down the highway.
    terminal_Write("GDT initialized. \n Before IDT. ");
    idt_Init(); // internal descriptor table - for internal CPU exceptions
    terminal_Write("IDT initialized. ");
   
    terminal_Write(" \n Before PIC remapping \n")
    pic_Remap(); //programmable interrupt controller - for external exceptions
    terminal_Write("PIC remap done. \n \n Before Virtual memory manager initialization \n");
 

    // filling the interrupt table (IDT) 
    // 
    registerInterruptHandler(32, irq0_Handler);
    registerInterruptHandler(0,isr0_Handler);
    terminal_Write("IDT filled. \n");


  //fix up the terminal writes up too, im 2 lazy rn
     vmm_Init();


    asm volatile("sti"); // enable interrupts.



    for (;;) {}
}

