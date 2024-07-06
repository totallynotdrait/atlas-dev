rm -r ./lib
cd ../gnu-efi && make bootloader
cd ../kernel && make kata && make buildimg
cd ../kernel