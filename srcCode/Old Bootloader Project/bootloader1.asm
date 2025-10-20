; bootloader1.asm - second stage bootloader
[org 0x7E00] ;When this code runs, it’ll be loaded at memory address 0x8000. So calculate all addresses and labels as if we started from there.

   


start:

 ; Reload sector 0 (boot sector) into 0x7C00
    mov ah, 0x02        ; BIOS read sectors
    mov al, 1           ; read 1 sector
    mov ch, 0           ; cylinder 0
    mov cl, 1           ; sector 1 (LBA 0)
    mov dh, 0           ; head 0
    mov dl, 0           ; drive 0
    mov bx, 0x7C00      ; store at 0x7C00
    int 0x13
    jc disk_error
    
    
    
    mov si, msg ; moves the ADDRESS of msg into the SI register ! MSG IS DEFINED A BIT LOWER BABE
.print_loop:
    lodsb                   ; Load byte from string into AL
    or al, al               ; Check if AL is zero (end of string)
    jz .after_msg           ; If zero, continue to FAT display
    mov ah, 0x0E            ; BIOS teletype output function
    int 0x10                ; BIOS interrupt to print char
    jmp .print_loop         ; Repeat

.after_msg:
   ;display some info from the FAT
    mov si, fatmsg          ; putting the fatmsg in the si register
    call print_string       ; prints the label message using this handy dandy function that loves the si register

    mov bx, 0x7C00          ; boot sector lives here! BIOS loaded it for us <3

    ; Read bytes per sector (offset 11, 2 bytes)
    mov ax, [bx + 11]
    call print_hex

    ; Read sectors per cluster (offset 13, 1 byte)
    xor ax, ax
    mov al, [bx + 13]
    call print_hex

    ; Read number of FATs (offset 16, 1 byte)
    xor ax, ax
    mov al, [bx + 16]
    call print_hex

    ; Read max root dir entries (offset 17, 2 bytes)
    mov ax, [bx + 17]
    call print_hex

  ;halt time
    cli                     ; Disable interrupts
    hlt                     ; Halt the CPU (infinite halt)
    jmp $                   ; Infinite loop 


; Print null-terminated string starting at DS:SI
print_string:
    lodsb
    or al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

; print_hex: print AX as a 4-digit uppercase hexadecimal number
print_hex:
    pusha
    mov cx, 4               ; loop 4 times (16 bits / 4 bits per digit)
.hex_loop:
    rol ax, 4               ; rotate highest nibble into lowest
    mov bl, al
    and bl, 0x0F
    add bl, '0'
    cmp bl, '9'
    jbe .print
    add bl, 7               ; turn 0xA–0xF into ASCII A–F
.print:
    mov ah, 0x0E
    mov al, bl
    int 0x10
    loop .hex_loop

    mov al, ' '             ; print a space between fields
    int 0x10
    popa
    ret

; DISK ERROR FOR LOADINNG DISK SPACE 0 AT THE START
disk_error:
    mov si, errormsg       ; point to error message
    call print_string      ; print it
    cli
    hlt
    jmp $

; data 
errormsg db 0x0D, 0x0A, 'Disk read error!', 0
msg db 'Hello from bootloader1!', 0
fatmsg db 0x0D, 0x0A, 'FAT12 fields: ', 0  ; newline then message




; Padding (optional atp)
times 2048 - ($ - $$) db 0   ;because you get 4 sectors, babe, 512 x 4

;did you know that the FAT12 boot sector (sector 0) is already loaded into RAM at 0x7C00 by the BIOS before your bootloader code even starts running. Which means its all loaded up and ready for ya. ain't that nice. just use it from the RAM now, since THE BIOS  super nicely already loaded the info.
