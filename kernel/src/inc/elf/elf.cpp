#include "elf.h"
#include <liba/stdio.h>

bool elf_check_file(Elf32_Ehdr *hdr) {
	if(!hdr) return false;
	if(hdr->e_ident[EI_MAG0] != ELFMAG0) {
		printf("internal_ata: cannot execute binary file: Exec format error (EI_MAG0)\n");
		return false;
	}
	if(hdr->e_ident[EI_MAG1] != ELFMAG1) {
		printf("internal_ata: cannot execute binary file: Exec format error (EI_MAG1)\n");
		return false;
	}
	if(hdr->e_ident[EI_MAG2] != ELFMAG2) {
		printf("internal_ata: cannot execute binary file: Exec format error (EI_MAG2)\n");
		return false;
	}
	if(hdr->e_ident[EI_MAG3] != ELFMAG3) {
		printf("internal_ata: cannot execute binary file: Exec format error (EI_MAG3)\n");
		return false;
	}
	return true;
}
