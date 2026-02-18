//interrupt descriptor table. To be used hand in hand with GDT, the global descriptor table.
#include "idt.h"

struct idt_Entry {
    uint16_t base_low;
    uint16_t selector;
    uint8_t  zero;
    uint8_t  flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_Ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

static struct idt_Entry idt[256];
static struct idt_Ptr   idtp;

extern void idt_Load(uint32_t);
extern void isr0();   // we’ll define this in assembly

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

    // Example: CPU exception 0 (Divide by Zero)
    idt_Set_Gate(0, (uint32_t)isr0, 0x08, 0x8E);

    idt_Load((uint32_t)&idtp);
}
