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

// use just for testing
void create_directory_example() {
    log->print("Initializing Ext2 FileSystem...");
    
    ATADevice* ataDevice = new ATADevice();
    log->info(to_hexstring((uint64_t)ataDevice));
    Ext2* ext2 = new Ext2(ataDevice);
    ext2->Init();


    vfs_node_t root;
    memset(&root, 0, sizeof(vfs_node_t));
    root.name[0] = '/';
    root.name[1] = '\0';
    root.inode_num = 2;
    root.size = 0;
    root.device = ataDevice;
    root.fs_type = VFS_EXT2_MAGIC;
    root.flags = FS_DIRECTORY;
    root.open_flags = 0;
    root.create_time = root.access_time = root.modified_time = 0;
    root.offset = 0;
    root.nlink = 2;
    root.refcount = 0;

    root.read = nullptr;
    root.write = nullptr;
    root.open = nullptr;
    root.close = nullptr;
    root.readdir = nullptr;
    root.finddir = nullptr;
    root.create = nullptr;
    root.unlink = nullptr;
    root.mkdir = nullptr;
    root.ioctl = nullptr;
    root.get_size = nullptr;
    root.chmod = nullptr;
    root.get_file_size = nullptr;
    root.listdir = nullptr;
    
    log->ok("Ext2 FileSystem is initialized successfully.");
}

extern "C" void _init_kata_main_process(BootInfo* bootInfo) {
    KAtaInfo kataInfo = InitializeKAta(bootInfo);

    // main
    log->ok("Atlas initialized succefully.");

    char* res;
    scanf(":: start ata shell?\n[y]es, [n]o /? %s", res);
    if (strcmp(res, "y") == 0) {
        init_ata_shell();
    } else {
        while (true);
    }
    
}