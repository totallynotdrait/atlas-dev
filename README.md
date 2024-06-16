# Atlas

A modern Shell-Like Operating System written in C and C++ with UEFI.
This repository is dedicated for development before release.
# Requirements
- CPU: A 64 bit cpu with at least 1 core.
- RAM: Any size is accetable, will fail to boot if it's or less than 40 mb.
- GPU: Intrigated or deticated is accetable.
- STORAGE: N/A.
- A UEFI Firmware, meaning the system must have a UEFI BIOS and not legacy.

# Compile Atlas

## Windows
1. run `run.bat` inside kernel folder

## Linux
1. run `build.sh` inside kernel folder


# Make a bootable device
Atlas can be booted via USB by following this steps:
1. Format the USB.
2. Create folder `EFI/BOOT`.
3. Copy `kata.elf` and it's system files (`zap-light16.psf`) into root dir.
4. Copy `main.efi` into `EFI/BOOT` and rename it to "BOOTX64.EFI".

If you want you can boot with floopy disks by doing the same steps for USB.
Altough is it not recommended to do it as it can have slow perfomance or random frezzes.
