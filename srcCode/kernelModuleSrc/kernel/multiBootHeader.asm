; multiBootHeader.asm (NASM syntax, 16KB stack, entry: _headerEntryPoint)

section .multiboot
align 4
    dd 0x1BADB002           ; magic
    dd 0x00                 ; flags
    dd -(0x1BADB002 + 0x00) ; checksum

section .text
global _headerEntryPoint
extern kmain

_headerEntryPoint:
    mov esp, stack_top
    push ebx        ; GRUB passes multiboot info pointer in EBX, so now we push this EBX pointer to our kmain
    call kmain
.hang:
    cli
    hlt
    jmp .hang

section .bss
align 4
stack_bottom:
    resb 16384              ; 16 KB stack
stack_top:
