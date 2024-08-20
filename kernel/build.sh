#rm -r ./lib # use this in some cases
cd ../gnu-efi && make bootloader
cd ../kernel && make kata && make buildimg
cd ../kernel
