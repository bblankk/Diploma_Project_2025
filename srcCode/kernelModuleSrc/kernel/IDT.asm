[bits 32]

global idt_Load
global irq0
global isr0

extern globalInterruptDelegator


idt_Load:

    mov eax, [esp+4]
    lidt [eax]
    ret



irq0:
    cli
    pusha
    push dword 0
    push dword 32
    mov eax, esp
    push eax
    call globalInterruptDelegator
    add esp, 4
    add esp, 8
    popa
    sti
    iretd



isr0:
    cli
    pusha
    push dword 0
    push dword 0
    mov eax, esp
    push eax
    call globalInterruptDelegator
    add esp, 4
    add esp, 8
    popa
    sti
    iretd