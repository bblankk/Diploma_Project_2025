global gdt_Flush

gdt_Flush:
    mov eax, [esp+4]
    lgdt [eax]

    mov ax, 0x10      ; Data selector (index 2 << 3)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush   ; Far jump to reload CS
.flush:
    ret
