/*
    KAta :: (Kernel ATA (ATA brevetation of Atlas))

    Originally written by Dr. AIT (AKAs: totallynotdrait, facilitycore, xourcx, 0x69)

    An advanced kernel with a simple renderer, basic I/O. Atlas bootloader and OS written with Unified Extensible Firmware Interface (UEFI).
    An Operating System so small that can be storred and booted in a 1.44 MB Floppy Disk the entire OS and it's system files.
    

    Note:
        Actually tested on floppy disk, it can give various problems like freezings and very slow perfomance.
    
    -- Credits
        A big thanks to Poncho for the UEFI OS development video tutorial.
        OSDev.org for the extra information about UEFI.
        Operating System Development Discord Server for the little help.
*/

#include "kataInit.h"
#include "shell/shell.h"
#include <fs/ext2.h>
#include <fs/vfs.h>
#include <drivers/ata/ata.h>
#include <drivers/ata/ata_device.h>
#include <liba/string.h>

extern "C" void _init_kata_main_process(BootInfo* bootInfo) {
    KAtaInfo kataInfo = InitializeKAta(bootInfo);

    // main
    log->ok("Atlas initialized succefully.");

    /* char* res;
    printf(":: start ata shell?\n[y]es, [n]o --: ");
    scanf("%s", res);
    if (strcmp(res, "y") == 0) {
        init_ata_shell();
    } else {
        while (true);
    } */

    init_ata_shell();

    // avoid computer to stop
    while (true)
    {
        asm __volatile__ ("hlt");
    }
    
    
}