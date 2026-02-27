# ==== Directories ====
SRCDIR = kernel
INCDIR = include
BUILDDIR = build
C_OBJS_DIR = $(BUILDDIR)/cobjs
ASM_OBJS_DIR = $(BUILDDIR)/asmobjs

# ==== Source Files ====
CFILES = \
	$(SRCDIR)/kernel.c \
	$(SRCDIR)/memap.c \
	$(SRCDIR)/terminal.c \
	$(SRCDIR)/bitmap.c \
	$(SRCDIR)/pmm.c \
	$(SRCDIR)/GDT.c \
	$(SRCDIR)/IDT.c \
	$(SRCDIR)/PIC.c \
	$(SRCDIR)/IRQHandler.c \
	$(SRCDIR)/interruptDelegator.c \
	$(SRCDIR)/vmm.c

ASMFILES = \
	$(SRCDIR)/multiBootHeader.asm \
	$(SRCDIR)/GDT.asm \
	$(SRCDIR)/IDT.asm

LDSCRIPT = linker.ld
OUT = $(BUILDDIR)/kernel.elf

# ==== Tools ====
CC = gcc
AS = nasm
LD = ld

# ==== Flags ====
CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra -c -I$(INCDIR)
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T $(BUILDDIR)/$(LDSCRIPT)

# ==== Object Files ====
COBJS = $(patsubst $(SRCDIR)/%.c,$(C_OBJS_DIR)/%.o,$(CFILES))
AOBJS = $(patsubst $(SRCDIR)/%.asm,$(ASM_OBJS_DIR)/%.o,$(ASMFILES))

# ==== Default Target ====
all: $(OUT)

# ==== Build Directories ====
$(BUILDDIR):
	mkdir -p $(BUILDDIR)
	mkdir -p $(C_OBJS_DIR)
	mkdir -p $(ASM_OBJS_DIR)

# ==== Copy Linker Script ====
$(BUILDDIR)/$(LDSCRIPT): $(LDSCRIPT) | $(BUILDDIR)
	cp $< $@

# ==== Assemble ====
$(ASM_OBJS_DIR)/%.o: $(SRCDIR)/%.asm | $(ASM_OBJS_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# ==== Compile C ====
$(C_OBJS_DIR)/%.o: $(SRCDIR)/%.c | $(C_OBJS_DIR)
	$(CC) $(CFLAGS) $< -o $@

# ==== Link ====
$(OUT): $(BUILDDIR)/$(LDSCRIPT) $(COBJS) $(AOBJS) | $(BUILDDIR)
	$(LD) $(LDFLAGS) -o $@ $(AOBJS) $(COBJS)

# ==== Run in QEMU ====
run: all
	qemu-system-i386 -kernel $(OUT)

# ==== Clean ====
clean:
	rm -rf $(BUILDDIR)