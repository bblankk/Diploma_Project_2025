// we're currently in protected real mode 32bit
//full access to address space :3
// apparently a stack pointer has been helpfully set by GRUB already. You can use stack - related C functions.
//currently in a nakey environment - just cpu , no libc, no bios calls.
// linker.ld is the roadmap for the kernel elf (which includes all files in the current working structure, btw)! 

#include <stddef.h>
#include <stdint.h> // implements fixed-width integer types int8_t, uint16_t, int32_t, and uint64_t (among others). 
 #include "terminal.h"  // "hey, my terminal functions are here!"
#include "multiboot.h"  //later defined - multiboot protocol. needed for mem. map
#include "memap.h" // memory map stuff
#include "pmm.h"

//kmain expects the ebx pointer (for the memory map) to be first argument on the stack. (pushed last)
void kmain(multibootInfo_t* mbInfo) {


    terminal_Initialize();
    terminal_Write("Terminal initialized\n");


    //initializing pmm and vmm's initial memory readings
    memap_Region regions[32]; ////parse, normalize, consume 
    int32_t regionCount; // the amount of available and normalized regions, ready to be used
    regionCount = parse_Map(mbInfo, regions, 32); //parse + fill regions[32]. Returns the amount of normalized type1 regions parsed.
    // todo print regino count
    regionCount = memap_Normalize(mbInfo, regions, 32); // page-align region base and length
    // todo print region count again to check its the same



    extern uint8_t kernelEnd; // from linker.ld , this is a pointer to the ADDRESS Of the end of the kernel
    uint64_t bitmapPhysicalBase = (uint64_t)&kernelEnd; // now this is the numerical value for the physical address where pmm's bitmap will live
   

    for (;;) {}
}

