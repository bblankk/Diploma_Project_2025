# Makefile for building your kernel in build/

# File names
ASM = multiBootHeader.asm
C   = kernel.c
LDSCRIPT = linker.ld
BUILDDIR = build
OUT = $(BUILDDIR)/kernel.elf

# Tools
CC = gcc
AS = nasm
LD = ld

# Flags
CFLAGS = -m32 -ffreestanding -c
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T $(BUILDDIR)/$(LDSCRIPT)

# Targets
all: $(OUT)

# Ensure build directory exists
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Copy linker script to build directory
$(BUILDDIR)/$(LDSCRIPT): $(LDSCRIPT) | $(BUILDDIR)
	cp $< $@

# Assemble multiboot header
$(BUILDDIR)/multiBootHeader.o: $(ASM) | $(BUILDDIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile kernel
$(BUILDDIR)/kernel.o: $(C) | $(BUILDDIR)
	$(CC) $(CFLAGS) $< -o $@

# Link everything
$(OUT): $(BUILDDIR)/multiBootHeader.o $(BUILDDIR)/kernel.o $(BUILDDIR)/$(LDSCRIPT)
	$(LD) $(LDFLAGS) -o $@ $(BUILDDIR)/multiBootHeader.o $(BUILDDIR)/kernel.o

run: all
	qemu-system-i386 -kernel $(OUT)

clean:
	rm -rf $(BUILDDIR)
