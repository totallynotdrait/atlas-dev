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
#include <scheduling/PIT/PIT.h>
#include <aagui/window/window.h>
#include <syscall/syscall.h>
#include <libc/include/stdio.h>

extern "C" void _init_kata_main_process(BootInfo* bootInfo) {
    KAtaInfo kataInfo = InitializeKAta(bootInfo);

    // main
    GKRenderer->Clear();
    log->ok("Atlas initialized succefully.");
    
    /* GKRenderer->Clear();
    GKRenderer->printf("[Welcome to "); GKRenderer->Color = bc.Cyan; GKRenderer->printf("Atlas-xUn"); GKRenderer->Color = bc.White; GKRenderer->printf("]");
    GKRenderer->Next();
    GKRenderer->printf("Atlas succefully booted to KAta with no errors, maybe.");
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("If mouse and keyboard are not working, it would probably because you're using a USB keyboard and mouse that Atlas doesn't currently support.");
    GKRenderer->Next();
    GKRenderer->printf("If for some reason boot-loops (which is impossible to read this text) it mostly cause because not enough sectors are read into memory.");
    GKRenderer->Next();
    GKRenderer->Next();
    GKRenderer->printf("You are using atlas-dev, which is a preview of atlas before a official release, but it may be unstable and cause crashes.");
    GKRenderer->Next();
    GKRenderer->Next(); */
    
    GKRenderer->printf("Press any key to continue..."); GKRenderer->Next();
    uint8_t uchar = getch();

    log->print(to_hexstring(uchar));

    while (true) {
        asm ("hlt");
    }
}