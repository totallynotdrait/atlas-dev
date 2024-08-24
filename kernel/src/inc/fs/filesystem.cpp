#include "filesystem.h"
#include <mem/memory.h>
#include <log.h>
#include <panic/panic.h>

Filesystem* filesystems[MAX_FILESYSTEMS];

void InitialiseFilesystems()
{
    memset(filesystems, 0, sizeof(Filesystem*) * MAX_FILESYSTEMS);
}

void RegisterFilesystem(Filesystem* filesystem)
{
    for(uint32_t i = 0; i < MAX_FILESYSTEMS; i++)
    {
        if(filesystems[i] == nullptr)
        {
            filesystems[i] = filesystem;
            return;
        }
    }

    Panic("EXCEEDED_MAX_FILESYSTEM", nullptr);
    while (true) {asm __volatile__ ("hlt");}
}

void UnregisterFilesystem(Filesystem* filesystem)
{
    for(uint32_t i = 0; i < MAX_FILESYSTEMS; i++)
    {
        if(filesystems[i] == filesystem)
        {
            filesystems[i] = nullptr;
            return;
        }
    }

    Panic("UNDEFINED_FILESYSTEM", nullptr);
    while (true) {asm __volatile__ ("hlt");}
}