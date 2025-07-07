; bootloader1.asm - second stage bootloader
[org 0x8000]

start:
    mov si, msg
.print_loop:
    lodsb                   ; Load byte from string into AL
    or al, al               ; Check if AL is zero (end of string)
    jz .done                ; If zero, end of string
    mov ah, 0x0E            ; BIOS teletype output function
    int 0x10                ; BIOS interrupt to print char
    jmp .print_loop         ; Repeat

.done:
    cli                     ; Disable interrupts
    hlt                     ; Halt the CPU (infinite halt)
    jmp $                   ; Infinite loop (you can also just leave it hanging here)

msg db 'Hello from bootloader1!', 0

; Pad to 512 bytes (optional, only if you want exact sector size)
times 512 - ($ - $$) db 0
