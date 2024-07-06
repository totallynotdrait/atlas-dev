#pragma once
#include "../kataUtil.h"
#include <kata_renderer.h>
#include <hardw/pci.h>
#include <log.h>


struct BootInfo;

class basesystem
{
    public:
        basesystem();
        static void broadcast(const char *message, const char *sender);
        void reboot();
        void shutdown();
        void execute_elf(const char* elf_path);
        void enableMCE();
        void enableFPU();
        void set_fpu_cw(const uint16_t cw);
};


