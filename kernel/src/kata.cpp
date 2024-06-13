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

//https://www.youtube.com/watch?v=e47SApmmx44&list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ&index=8&ab_channel=Poncho
//(36:08)

#include "kataUtil.h"

extern "C" void _init_kata_main_process(BootInfo* bootInfo) {

    KAtaInfo kataInfo = InitializeKAta(bootInfo);
    PageTableManager* pageTableManager = kataInfo.pageTableManager;
    GKRenderer->printf("Atlas and KAta booted successfully, infjmp init...");
    GKRenderer->Next();
    GKRenderer->printf(to_hexstring((uint64_t)bootInfo->rsdp));    
    GKRenderer->Next();


    while (true)
    {
        ProcessMousePacket();
    }
    

    while (true);
}