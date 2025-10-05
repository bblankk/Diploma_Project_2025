; bootloader0.asm - first stage bootloader (512 bytes)
[org 0x7C00]

start:
    mov ax, 0              ; Clear the registers. memory segmentation in real mode (startup mode) uses these registers to access the memory, so we'll need them clean.
    mov ds, ax ; datta segment (ds) and      by the way, mov is a copy (dsetination) (source) function.
    mov es, ax ; extra segment (es) to zero.  ; by the way, you need to go throu a general purpose reg (like ax) before writing to ds or es

    ; BIOS read 1 sector from cylinder=0, head=0, sector=2 into 0x0000:0x8000  - this uses CHA scheme for old devices... don't worry about it
    mov ah, 0x02            ; BIOS read sectors
    mov al, 4               ; read the next 4 sectors (2 to 5) (i've decided that'll be the size of our stage2 bootloader.
    mov ch, 0               ; cylinder 0
    mov cl, 2               ; sector 2
    mov dh, 0               ; head 0 (floppy)
    mov dl, 0               ; drive 0 (floppy)
    mov bx, 0x7E00         ; offset in memory (0x8000) where stage 2 will be set
    int 0x13                ; BIOS interrupt for disk operations
    jc disk_error           ; jump if error (carry set)
    
;to sum up the top thing: Hi BIOS, please read 1 sector from the disk and dump it into memory at 0x0000:0x8000.

    ; Jump to second stage (bootloader1) at 0x8000
    jmp 0x0000:0x8000

disk_error:
    mov si, err_msg         ; Error message string
.print_loop:
    lodsb                   ; Load byte from string into AL
    or al, al               ; Check if AL is zero (end of string)
    jz $                    ; Jump if zero (end of string)
    mov ah, 0x0E            ; BIOS teletype output function
    int 0x10                ; BIOS interrupt to print char
    jmp .print_loop         ; Repeat

err_msg db 'Disk read error!',0

times 510 - ($ - $$) db 0   ; Fill remaining space with zeros
dw 0xAA55                   ; Boot signature

