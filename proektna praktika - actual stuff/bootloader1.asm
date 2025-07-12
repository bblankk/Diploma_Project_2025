; bootloader1.asm - second stage bootloader
[org 0x8000] ;When this code runs, it’ll be loaded at memory address 0x8000. So calculate all addresses and labels as if we started from there.

start:
    mov si, msg ; moves the ADDRESS of msg into the SI register ! MSG IS DEFINED A BIT LOWER BABE
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
times 2048 - ($ - $$) db 0   ;because you get 4 sectors, babe, 512 x 4
