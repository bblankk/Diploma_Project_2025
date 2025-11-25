# Makefile for building your kernel in build/

# Directories
SRCDIR = kernel
INCDIR = include
BUILDDIR = build

# Source files
ASM = $(SRCDIR)/multiBootHeader.asm
CFILES = $(SRCDIR)/kernel.c $(SRCDIR)/memap.c $(SRCDIR)/terminal.c
LDSCRIPT = linker.ld
OUT = $(BUILDDIR)/kernel.elf

# Tools
CC = gcc
AS = nasm
LD = ld

# Flags
CFLAGS = -m32 -ffreestanding -c -I$(INCDIR)
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T $(BUILDDIR)/$(LDSCRIPT)

# Object files
COBJS = $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(CFILES))
AOBJS = $(BUILDDIR)/multiBootHeader.o

# Default target
all: $(OUT)

# Ensure build directory exists
$(BUILDDIR):
	mkdir -p $(BUILDDIR)

# Copy linker script to build directory
$(BUILDDIR)/$(LDSCRIPT): $(LDSCRIPT) | $(BUILDDIR)
	cp $< $@

# Assemble .asm files
$(BUILDDIR)/%.o: $(SRCDIR)/%.asm | $(BUILDDIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile C files
$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) $< -o $@

# Link everything
$(OUT): $(AOBJS) $(COBJS) $(BUILDDIR)/$(LDSCRIPT)
	$(LD) $(LDFLAGS) -o $@ $(AOBJS) $(COBJS)

# Run in QEMU
run: all
	qemu-system-i386 -kernel $(OUT)

# Clean build
clean:
	rm -rf $(BUILDDIR)
