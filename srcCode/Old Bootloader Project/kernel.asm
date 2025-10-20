; kernel.asm - very basic kernel
[org 0x1000]  ; Kernel loads at 0x1000 in memory

start:
    mov si, msg
.print_loop:
    lodsb                    ; Load byte from string into AL
    or al, al                ; Check if AL is zero (end of string)
    jz .done                 ; If zero, end of string
    mov ah, 0x0E             ; BIOS teletype output function
    int 0x10                 ; BIOS interrupt to print char
    jmp .print_loop          ; Repeat

.done:
    jmp .done                 ; Infinite loop to keep kernel running

msg db 'Hello from kernel!', 0
