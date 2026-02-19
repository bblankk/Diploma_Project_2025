/*
if the idt is the guide for internal interrupts (cpu exceptions),
but hardware devices (timer, keyboard, etc.) generate external interrupts,
then the PIC (programmable interrupt handler) handles these external interrupts.
This is a chip on the motherboard the CPU runs to when there's an external interrupt.
We have to reprogram it so it doesn't conflict with the CPU interrupt numbers. (allegedly,it currently does.)
*/
/*
0–31 = CPU exceptions
32–47 = hardware interrupts
timer IRQ0 ? IDT[32] ? timer handler
*/
// PIT (programmer interval timer ) also just known as the timer or internal clock, is crucial to multitasking and used by the scheduler. Everybody say thank you, PIT(bull)!
/*
A latent lesson in portByteOut and portByteIn:
For the CPU to talk to external devices, it sends and receives signals through a little gate at the socket where that external device is connected. For example, there's one such gate at every USB port.
portByteOut - send 1 byte to hardware port
portByteIn -  read 1 byte from hardware port
*/


#include <stdint.h>

#define PIC1        0x20
#define PIC2        0xA0
#define PIC1_COMMAND PIC1
#define PIC1_DATA   (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA   (PIC2+1)

#define ICW1_INIT   0x10
#define ICW1_ICW4   0x01
#define ICW4_8086   0x01

// send value 'value' to hardware port 'port'
static inline void portByteOut(uint16_t port, uint8_t value)
{
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}


static inline uint8_t portByteIn(uint16_t port)
{
    uint8_t result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

void pic_Remap(void)
{
    uint8_t a1 = portByteIn(PIC1_DATA);
    uint8_t a2 = portByteIn(PIC2_DATA);

    portByteOut(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    portByteOut(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    portByteOut(PIC1_DATA, 0x20); // Master offset 32
    portByteOut(PIC2_DATA, 0x28); // Slave offset 40

    portByteOut(PIC1_DATA, 4);
    portByteOut(PIC2_DATA, 2);

    portByteOut(PIC1_DATA, ICW4_8086);
    portByteOut(PIC2_DATA, ICW4_8086);

    portByteOut(PIC1_DATA, a1);
    portByteOut(PIC2_DATA, a2);
}

// this is called when the exception has been handled , to let the CPU know it can proceed! VERY important if you don't want PIC to get stuck inside an interrupt!!
//this is only needed for external interrupts, as internal (CPU) interrupts have their own way of knowing when they're done.
void pic_SendEOI(unsigned char irq)
{
    if (irq >= 8)
        portByteOut(PIC2_COMMAND, 0x20);

    portByteOut(PIC1_COMMAND, 0x20);
}
