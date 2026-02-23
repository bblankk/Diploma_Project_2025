# Makefile for building your kernel in build/

# Directories
SRCDIR = kernel
INCDIR = include
BUILDDIR = build
C_OBJS_DIR = $(BUILDDIR)/cobjs
ASM_OBJS_DIR = $(BUILDDIR)/asmobjs

# Source files
ASM = $(SRCDIR)/multiBootHeader.asm $(SRCDIR)/gdt.asm $(SRCDIR)/idt.asm
CFILES = $(SRCDIR)/kernel.c \
         $(SRCDIR)/memap.c \
         $(SRCDIR)/terminal.c \
         $(SRCDIR)/bitmap.c \
         $(SRCDIR)/pmm.c \
         $(SRCDIR)/gdt.c \
         $(SRCDIR)/idt.c \
         $(SRCDIR)/pic.c \
         $(SRCDIR)/timer.c \
         $(SRCDIR)/isr.c

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
COBJS = $(patsubst $(SRCDIR)/%.c,$(C_OBJS_DIR)/%.o,$(CFILES))
AOBJS = $(patsubst $(SRCDIR)/%.asm,$(ASM_OBJS_DIR)/%.o,$(ASM))

# Default target
all: $(OUT)

# Create necessary directories
$(BUILDDIR):
	mkdir -p $(BUILDDIR)
	mkdir -p $(C_OBJS_DIR)
	mkdir -p $(ASM_OBJS_DIR)

# Copy linker script
$(BUILDDIR)/$(LDSCRIPT): $(LDSCRIPT) | $(BUILDDIR)
	cp $< $@

# Assemble ASM files
$(ASM_OBJS_DIR)/%.o: $(SRCDIR)/%.asm | $(ASM_OBJS_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile C files
$(C_OBJS_DIR)/%.o: $(SRCDIR)/%.c | $(C_OBJS_DIR)
	$(CC) $(CFLAGS) $< -o $@

# Link everything
$(OUT): $(BUILDDIR)/$(LDSCRIPT) $(COBJS) $(AOBJS) | $(BUILDDIR)
	$(LD) $(LDFLAGS) -o $@ $(AOBJS) $(COBJS)

# Run in QEMU
run: all
	qemu-system-i386 -kernel $(OUT)

# Clean build folders
clean:
	rm -rf $(BUILDDIR)