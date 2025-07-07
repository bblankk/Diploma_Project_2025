; bootloader1.asm - second stage bootloader
[org 0x8000]

start:
    mov si, msg
.print_loop:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp .print_loop

.done:
    cli
    hlt
    jmp $

msg db 'Hello from bootloader1!', 0

; Pad to 512 bytes (optional, only if you want exact sector size)
times 512 - ($ - $$) db 0
