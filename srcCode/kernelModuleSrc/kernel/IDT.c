//interrupt descriptor table. To be used hand in hand with GDT, the global descriptor table.
//It does not handle interrupts. It only defines where each interrupt points.
/* 
responsibilities:
build IDT entries (a .. list, map, of just pointers to different handlers. They will probably be defined at IRQHandler.c)
set addresses of interrupt requests (irqs)
load IDT

-- without this, the CPU won't know where irq0 lives. 
*/
#include "idt.h"

/*
every idt entry is called a 'gate descriptor'. A gate contains the stuff below. 
*/
struct idt_Entry {
    uint16_t base_low; //address to jump to
    uint16_t selector; //segment selector
    uint8_t  zero; //type : itnerrupt or trap gate. don't ask me about trap gates.
    uint8_t  flags; //flags 
    uint16_t base_high;
} __attribute__((packed));

struct idt_Ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

//variables
static struct idt_Entry idt[256]; //idt entry
static struct idt_Ptr idtp; //idt pointer to entry
extern void idt_Load(uint32_t);
extern void isr0();   // divide by 0
extern void irq0();  // ... timer? i think.


//basically writes a gate based on the descriptor. A gate, mind you, is a glorified pointer with a litle bit more information.
static void idt_Set_Gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags)
{
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;

    idt[num].selector  = sel;
    idt[num].zero      = 0;
    idt[num].flags     = flags;
}

void idt_Init(void)
{
    idtp.limit = sizeof(idt) - 1;
    idtp.base  = (uint32_t)&idt;

    // Clear table
    for (int i = 0; i < 256; i++)
        idt_Set_Gate(i, 0, 0, 0);

    // CPU exception 0 (Divide by Zero)
    idt_Set_Gate(0, (uint32_t)isr0, 0x08, 0x8E);
    // timer interrupt 32
    idt_Set_Gate(32, (uint32_t)irq0, 0x08, 0x8E);

    idt_Load((uint32_t)&idtp);
}
