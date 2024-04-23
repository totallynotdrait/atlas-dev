@echo off

echo [Compiling Bootloader]
nasm bootloader/bootloader.asm -o build/bootloader.bin
nasm bootloader/ExtendedProgram.asm -f bin -o build/ExtendedProgram.bin

copy /b build/bootloader.bin+ExtendedProgram.bin "output/bootloader.flp"


start bochsrc.bxrc