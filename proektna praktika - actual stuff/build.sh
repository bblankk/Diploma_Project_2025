set -e 

echo "Building..."

if [ ! -d "newBinFiles" ]; then
    mkdir newBinFiles
    echo "Created folder: newBinFiles"
else
    echo "Folder already exists: newBinFiles"
fi

echo
echo "Assembling..."
nasm bootloader0.asm -o bootloader0.bin
nasm bootloader1.asm -o bootloader1.bin
nasm kernel.asm      -o kernel.bin

mv -f bootloader0.bin bootloader1.bin kernel.bin newBinFiles/
echo ".bin files moved to newBinFiles/"




echo "Creating FAT12 floppy image"
# Make empty 1.44MB image
dd if=/dev/zero of=floppy.img bs=512 count=2880 status=none

# Format as FAT12
mkfs.fat -F 12 floppy.img > /dev/null

echo "Adding kernel to FAT filesystem..."
mkdir -p mnt
sudo mount -o loop floppy.img mnt

sudo cp newBinFiles/kernel.bin      mnt/KERNEL.BIN

sudo umount mnt
rmdir mnt

# Write bootloader0 directly to sector 0
dd if=newBinFiles/bootloader0.bin of=floppy.img bs=512 count=1 conv=notrunc status=none
echo "bootloader0 written to sector 0"
# Write bootloader1 directly to sectors 2-5
dd if=newBinFiles/bootloader1.bin of=floppy.img bs=512 seek=2 count=4 conv=notrunc
echo "bootloader1 written to sector 2-5"

# Step 4: Run with QEMU
echo
echo "Launching in QEMU.."
qemu-system-x86_64 -fda floppy.img -d int,cpu_reset -no-reboot
echo "Done!"




