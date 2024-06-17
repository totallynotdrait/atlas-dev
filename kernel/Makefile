
OSNAME = atlas-dev_xUn

GNUEFI = ../gnu-efi
OVMFDIR = ../OVMFbin
LDS = kata.ld
CC = gcc
ASMC = nasm
LD = ld

CFLAGS = -ffreestanding -fshort-wchar -mno-red-zone -I$(GNUEFI)/inc -I$(GNUEFI)/inc/x86_64 -Isrc -Isrc/log -Isrc/inc -Isrc/kata_renderer
ASMFLAGS = 
LDFLAGS = -T $(LDS) -static -Bsymbolic -nostdlib

SRCDIR := src
OBJDIR := lib
BUILDDIR = bin
BOOTEFI := $(GNUEFI)/x86_64/bootloader/main.efi

rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,$(SRCDIR),*.cpp)
ASMSRC = $(call rwildcard,$(SRCDIR),*.asm)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRC))
OBJS += $(patsubst $(SRCDIR)/%.asm, $(OBJDIR)/%_asm.o, $(ASMSRC))
DIRS = $(wildcard $(SRCDIR)/*)

kata: $(OBJS) link

$(OBJDIR)/kata_renderer/kata_renderer.o: $(SRCDIR)/kata_renderer/kata_renderer.cpp
	@echo "\e[91m[Compiling $^]\e[0m"
	@mkdir -p $(@D)
	$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -I$(GNUEFI)/inc -I$(GNUEFI)/inc/x86_64 -Isrc/log -Isrc/inc -Isrc/kata_renderer -Isrc -c $^ -o $@

$(OBJDIR)/inc/interrupts/interrupts.o: $(SRCDIR)/inc/interrupts/interrupts.cpp
	@echo "\e[91m[Compiling $^]\e[0m"
	@mkdir -p $(@D)
	$(CC) -mno-red-zone -mgeneral-regs-only -ffreestanding -I$(GNUEFI)/inc -I$(GNUEFI)/inc/x86_64 -Isrc/log -Isrc/inc -Isrc/kata_renderer -Isrc -c $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo "\e[91m[Compiling $^]\e[0m"
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $^ -o $@

$(OBJDIR)/%_asm.o: $(SRCDIR)/%.asm
	@echo "\e[91m[Compiling $^]\e[0m"
	@mkdir -p $(@D)
	$(ASMC) $(ASMFLAGS) $^ -f elf64 -o $@

link:
	@echo "\e[91m[Linking]\e[0m"
	$(LD) $(LDFLAGS) -o $(BUILDDIR)/kata.elf $(OBJS)

setup:
	@mkdir $(BUILDDIR)
	@mkdir $(SRCDIR)
	@mkdir $(OBJDIR)

buildimg:
	@echo "\e[91m[Building image]\e[0m"
	dd if=/dev/zero of=$(BUILDDIR)/$(OSNAME).img bs=512 count=93750
	mformat -i $(BUILDDIR)/$(OSNAME).img ::
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI
	mmd -i $(BUILDDIR)/$(OSNAME).img ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BOOTEFI) ::/EFI/BOOT
	mcopy -i $(BUILDDIR)/$(OSNAME).img startup.nsh ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/kata.elf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/zap-vga16.psf ::
	mcopy -i $(BUILDDIR)/$(OSNAME).img $(BUILDDIR)/zap-light16.psf ::

run:
	qemu-system-x86_64 -drive file=$(BUILDDIR)/$(OSNAME).img -m 256M -cpu qemu64 -drive if=pflash,format=raw,unit=0,file="$(OVMFDIR)/OVMF_CODE-pure-efi.fd",readonly=on -drive if=pflash,format=raw,unit=1,file="$(OVMFDIR)/OVMF_VARS-pure-efi.fd" -net none
