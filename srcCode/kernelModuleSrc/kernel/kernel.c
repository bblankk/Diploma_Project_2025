// we're currently in protected real mode 32bit
//full access to address space :3
// apparently a stack pointer has been helpfully set by GRUB already. You can use stack - related C functions.
//currently in a nakey environment - just cpu , no libc, no bios calls.


#include <stddef.h>
#include <stdint.h> // implements fixed-width integer types int8_t, uint16_t, int32_t, and uint64_t (among others). 
 #include "terminal.h"  // "hey, my terminal functions are here!"
#include "multiboot.h"  //later defined - multiboot protocol. needed for mem. map
#include "memap.h" //memory map stuff



//main 
//kmain expects the ebx pointer (for the memory map) to be first argument on the stack. (pushed last)
void kmain(multibootInfo_t* mbInfo) {


    terminal_Initialize();
    terminal_Write("Hello, world from kernel!\n");
    terminal_Write("This is a new line!\n");
    terminal_Write("Nice, right?\n \n");

    // Print memory map passed by GRUB
    print_Memory_Map(mbInfo) ;


    for (;;) {}
}

