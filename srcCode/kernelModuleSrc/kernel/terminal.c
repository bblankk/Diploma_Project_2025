//HERE LIVE THE TERMINAL FUNCTIONALITY

#include <stddef.h>
#include <stdint.h>
#include "terminal.h"


// # - library, be careful with which ones work. no C libraries only definitions
// "" - this is MY OWN header, that i made . compiled with your kernel.
//GLOBALS
#define VGA_WIDTH 80  
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000  //starting ADDRESS of the terminal frame buffer


// VGA/TERMINAL STUFF
static uint16_t* const VGA_BUFFER = (uint16_t*)VGA_ADDRESS;  //each address has color and value -2 bytes = 16bits , const = we're not changing where it's pointing, it'll always point to 0xB8000, static = file-scoped, only available in this file.
//this is the buffer pointer.
//VGA_BUFFER[index] = something; writes directly to video memory

//global storage - FOR NOWWWW SOBSOB
static size_t terminal_row;  
static size_t terminal_column;
static uint8_t terminal_color;

static inline uint8_t vga_entry_color(uint8_t fg, uint8_t bg) {
    return fg | bg << 4;
}
//fg goes into the lower 4 bits, bg goes into the higher 4 bits
// inline = insert directly where it's used (no func call overhead)
// static = file-scoped
//this whole func operates a single byte that encodes COLOR
//vga_entry('A', 0x07) == 0x0741  ; example color allocation

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}


void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(7, 0); // initial terminal color, grey on black
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_BUFFER[index] = vga_entry(' ', terminal_color);
        }
    }
}
//This sets your cursor to (0, 0), picks a color, and fills the entire screen with spaces.
//It’s writing ' ' and 0x07 into every cell of that 0xB8000 buffer.aka initialization of the terminal

void terminal_printchar(char c) {  //used to be terminalputchar
    if (c == '\n') {
        terminal_row++;
        terminal_column = 0;   // is it a new line? no? skip
        if (terminal_row >= VGA_HEIGHT)
            terminal_row = 0; // later: scroll
        return;
    }

    const size_t index = terminal_row * VGA_WIDTH + terminal_column;   //calc memory offset, store in "index" 
    VGA_BUFFER[index] = vga_entry(c, terminal_color);  //write to vga buffer

    terminal_column++;
    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row >= VGA_HEIGHT)
            terminal_row = 0;
    }
}
//printing CHARACTERS

void terminal_write(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++)
        terminal_printchar(data[i]);
}
//loops through each byte until \0 (end of string) and prints it.


//reading memory map passed to us by GRUB bootloader.


