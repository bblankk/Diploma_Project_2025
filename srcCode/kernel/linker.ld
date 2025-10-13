ENTRY(kmain)

SECTIONS
{
  /* Load the kernel at the 1MB mark (GRUB’s default safe zone) */
  . = 1M;

  /* The multiboot header must be in the first 8KB of the file */
  .multiboot :
  {
    *(.multiboot)
  }

  .text ALIGN(4K) :
  {
    *(.text*)
  }

  .rodata ALIGN(4K) :
  {
    *(.rodata*)
  }

  .data ALIGN(4K) :
  {
    *(.data*)
  }

  .bss ALIGN(4K) :
  {
    *(COMMON)
    *(.bss*)
  }
}

