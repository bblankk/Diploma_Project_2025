#!/bin/bash
set -e  # Exit on error

echo "Building..."

# Step 0: Create output directory
if [ ! -d "newBinFiles" ]; then
    mkdir newBinFiles
    echo "Created folder: newBinFiles"
else
    echo "Folder already exists: newBinFiles"
fi

# Step 1: Assemble all .asm files with NASM
echo
echo "[1/3] Assembling..."
nasm bootloader0.asm -o bootloader0.bin
nasm bootloader1.asm -o bootloader1.bin
nasm kernel.asm      -o kernel.bin

mv -f *.bin newBinFiles/
echo "All done. .bin files moved to newBinFiles."

# Step 2: Create floppy.img
echo "[2/3] Creating disk image..."
rm -f floppy.img
cat newBinFiles/bootloader0.bin newBinFiles/bootloader1.bin newBinFiles/kernel.bin > floppy.img

# Step 3: Run in QEMU
echo "[3/3] Running in QEMU..."
qemu-system-x86_64 -fda floppy.img

echo
echo "Done!"

