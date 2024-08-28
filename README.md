# Atlas

Atlas is a modern Unix Shell-Like Operating System written in C++ with a UEFI Bootloader.
This repository is dedicated for development before release.

- [x] Basic kernel drivers (Keyboard, Mouse, Framebuffer, ATA, AHCI, ...)
- [x] EFI Bootloader
- [ ] BIOS Bootloader
- [x] Create and initialize IDT for interrupts
- [x] Paging
- [x] kheap
- [x] Create a simple renderer and font renderer
- [ ] Jump to Userspace
- [x] Implement printf and scanf functions similar to stdio.h
- [x] Enable FPU
- [x] Enable Machine Check
- [x] Make a panic screen with all possible faults/traps
- [x] System calls with Interrupt Vector 0x80
- [x] Basic library for drawing basic shapes
- [ ] Gaussian Blur algorithm
- [x] Basic Shell
- [ ] Add a filesystem like Ext2 or FAT32 and VFS
- [x] PCI hardware detection
- [x] USB 1.0 (UHCI) Drivers
- [x] Internet drivers for Intel i217
- [ ] Internet drivers for RTL8139
- [ ] ELF Execution
- [x] Process creation
- [x] Boot with GRUB (see this wiki)[https://github.com/totallynotdrait/atlas-dev/wiki/Make-a-ISO-Image-and-boot-Atlas-with-GRUB]
- [ ] USB Keyboard and Mouse
- [ ] 
Atlas Public Development: https://trello.com/w/atlaspublicdev
