xorriso -as mkisofs \
		-J \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot efi/boot/bootx64.efi \
		-efi-boot-part --efi-boot-image \
		iso_root -o atlas-dev.iso