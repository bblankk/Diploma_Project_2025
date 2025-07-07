; bootimage.asm - Combines bootloader0 and bootloader1
[org 0x7C00]

    ; This file doesn't do anything at runtime.
    ; It just serves as a container for the two stages.

    ; First stage bootloader
    incbin "bootloader0.bin"

    ; Second stage bootloader (next sector)
    incbin "bootloader1.bin"
