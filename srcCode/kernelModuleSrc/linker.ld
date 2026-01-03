ENTRY(_headerEntryPoint)
SECTIONS
{
    . = 1M;
    kernelStart = .;

    .text : {
        *(.multiboot)
        *(.text*)
    }

    .rodata : { *(.rodata*) }
    .data : { *(.data*) }
    .bss : { *(.bss*) *(COMMON) }

    kernelEnd = .;
}
