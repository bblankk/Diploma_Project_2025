; bootloader0.asm - first stage bootloader (512 bytes)
[org 0x7C00]

start:
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; BIOS read 1 sector from cylinder=0, head=0, sector=2 into 0x0000:0x8000
    mov ah, 0x02      ; BIOS read sectors
    mov al, 1         ; read 1 sector
    mov ch, 0         ; cylinder 0
    mov cl, 2         ; sector 2
    mov dh, 0         ; head 0
    mov dl, 0         ; drive 0 (floppy)
    mov bx, 0x8000    ; offset
    int 0x13
    jc disk_error     ; jump if error (carry set)

    jmp 0x0000:0x8000 ; jump to bootloader1

disk_error:
    mov si, err_msg
.print_loop:
    lodsb
    or al, al
    jz $
    mov ah, 0x0E
    int 0x10
    jmp .print_loop

err_msg db 'Disk read error!',0

times 510 - ($ - $$) db 0
dw 0xAA55





;how to run :"C:\Program Files\qemu\qemu-system-x86_64.exe" -drive format=raw,file=bootloader.img 
; i've already assembled it with nasm, beware it only works (nasm( in this folder (downloads). assemble with care.
;you need to specify full path to qemu exe if you want it to run. No adding to PATH nonsense! 
; dont worry about the drive formatting forn ow. 
; here's your sources : https://www.qemu.org/docs/master/system/invocation.html, https://flatassembler.net/examples.php, https://mh-nexus.de/en/hxd/ ,,

;qemu can be run to simulate wherever
; nasm can be run anywhere, but you first need to copy nasmpath and run it there LMAO
