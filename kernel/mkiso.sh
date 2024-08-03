mkdir -p iso_root/EFI/BOOT
cp ../gnu-efi/x86_64/bootloader/bootx64.efi iso_root/EFI/BOOT
cp bin/kata.elf iso_root
cp bin/zap-vga16.psf iso_root
cp bin/zap-light16.psf iso_root

xorriso -as mkisofs \
		-J \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot efi/boot/bootx64.efi \
		-efi-boot-part --efi-boot-image \
		iso_root -o atlas-dev.iso