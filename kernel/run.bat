set OSNAME=atlas-dev_xUn
set BUILDDIR=%0/../bin
set OVMFDIR=%0/../../OVMFbin

qemu-system-x86_64 -machine q35 -drive file=%BUILDDIR%/%OSNAME%.img -m 8G -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="%OVMFDIR%/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="%OVMFDIR%/OVMF_VARS-pure-efi.fd" -netdev user,id=net0 -device e1000,netdev=net0