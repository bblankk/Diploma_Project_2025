#include <stdint.h>
//for confusion with the assembly, this site may help
// https://www.cs.virginia.edu/~evans/cs216/guides/x86.html

//this is an I/O port for the COM1 serial port. Commonly used for serial connection.
#define COM1_PORT 0x3F8 //starting address of the com1 port

static void output_Byte_To_Port(uint16_t port, uint8_t val) {
    //this is an inline assembly block 
    //val goes into register a (lower 8 bytes of eax, aka AL, to be specific)
    //port goes to port (Don't worry about Nd, it's some cocophony of an unint and register...depends on whether the port number can be accessed immediately. Er, don't worry about it, it shouldn't bite.
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static uint8_t input_Byte_From_Port(uint16_t port) {
    uint8_t returnValue;
    // ""=a" means it's reading from eax, as opposed to "a" that's writing to it
    //you may notice the similarity to output_Byte_To_Port, just reversed.
    __asm__ volatile ("inb %1, %0" : "=a"(returnValue) : "Nd"(port));
    return returnValue;
}

// Check if data is available to read - returns 0 if ready, 1 if not ready
int serial_Is_Data_Ready_COM1_Port() {
    return input_Byte_From_Port(COM1_PORT + 5) & 1; // LSR register's Data Ready bit is bit 0 - if bit 0 is set to 1, it's ready, if it's set to 0, it's not ready.
}

char serial_Read_Char_From_Port() {
    while (!serial_Is_Data_Ready_COM1_Port()); // Wait until data is ready
    return input_Byte_From_Port(COM1_PORT);           // Read received byte from data register
}

void check_com1_and_report() {
    if (serial_Is_Data_Ready_COM1_Port()) {
        char c = serial_Read_Char_From_Port();
        // If you want to base the message on receiving any data
        terminalWrite("com1 is up\n");
        // You could extend this to check for specific data patterns here
    }
}