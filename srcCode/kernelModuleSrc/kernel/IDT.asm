[bits 32]

global idt_Load
idt_Load:
    mov eax, [esp+4]
    lidt [eax]
    ret

global isr0
extern isr0_handler

isr0:
    cli
    pusha
    call isr0_handler
    popa
    sti
    iretd
