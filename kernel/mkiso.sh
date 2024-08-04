mkdir -p iso/EFI/BOOT
cp ../gnu-efi/x86_64/bootloader/bootx64.efi iso/EFI/BOOT
cp bin/kata.elf iso
cp bin/zap-vga.psf iso

grub-mkrescue -o atlas-dev.iso iso