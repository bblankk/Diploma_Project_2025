; bootloader0.asm - first stage bootloader (512 bytes)
[org 0x7C00]

start:
    xor ax, ax              ; Clear the registers
    mov ds, ax
    mov es, ax

    ; BIOS read 1 sector from cylinder=0, head=0, sector=2 into 0x0000:0x8000
    mov ah, 0x02            ; BIOS read sectors
    mov al, 1               ; read 1 sector
    mov ch, 0               ; cylinder 0
    mov cl, 2               ; sector 2
    mov dh, 0               ; head 0
    mov dl, 0               ; drive 0 (floppy)
    mov bx, 0x8000          ; offset in memory (0x8000)
    int 0x13                ; BIOS interrupt for disk operations
    jc disk_error           ; jump if error (carry set)

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





;how to run :"C:\Program Files\qemu\qemu-system-x86_64.exe" -drive format=raw,file=bootloader.img 
; i've already assembled it with nasm, beware it only works (nasm( in this folder (downloads). assemble with care.
;you need to specify full path to qemu exe if you want it to run. No adding to PATH nonsense! 
; dont worry about the drive formatting forn ow. 
; here's your sources : https://www.qemu.org/docs/master/system/invocation.html, https://flatassembler.net/examples.php, https://mh-nexus.de/en/hxd/ ,,

;qemu can be run to simulate wherever
; nasm can be run anywhere, but you first need to copy nasmpath and run it there LMAO
