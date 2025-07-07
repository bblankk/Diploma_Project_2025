@echo off
setlocal enabledelayedexpansion

:: Colors (only on Windows 10+)
for /f "delims=" %%i in ('"prompt $H & for %%b in (1) do rem"') do set "BS=%%i"
<nul set /p "=Building..." >nul

if not exist newBinFiles (
    mkdir newBinFiles
    echo Created folder: newBinFiles
) else (
    echo Folder already exists: newBinFiles
)


:: Step 1: Assemble all .asm files with FASM
echo.
echo [1/3] Assembling...
nasm bootloader0.asm -o bootloader0.bin || goto error
nasm bootloader1.asm -o bootloader1.bin || goto error
nasm kernel.asm      -o kernel.bin      || goto error

move /Y *.bin newBinFiles\
echo All done. .bin files moved to newBinFiles.

:: Step 2: Create floppy.img
echo [2/3] Creating disk image...
del floppy.img >nul 2>&1
copy /b newBinFiles\bootloader0.bin + newBinFiles\bootloader1.bin + newBinFiles\kernel.bin floppy.img >nul || goto error


:: Step 3: Run in QEMU
echo [3/3] Running in QEMU...
"C:\Program Files\qemu\qemu-system-x86_64.exe" -fda floppy.img

echo.
echo Done!
goto end

:error
echo.
echo [!] Build failed.
pause
:end
