[bits 32]

global idt_Load
global irq0
global isr0

extern irq0_Handler
extern isr0_Handler

idt_Load:
    mov eax, [esp+4]
    lidt [eax]
    ret

isr0:
    cli
    pusha
    call isr0_Handler
    popa
    sti
    iretd

irq0:
    cli
    pusha
    call irq0_Handler
    popa
    sti
    iretd
