#pragma once
#include "../kata_renderer/kata_renderer.h"
#include "../kataUtil.h"
#include "hardw/pci.h"
#include <log.h>


struct BootInfo;

class basesystem
{
    public:
        basesystem();
        static void broadcast(const char *message, const char *sender);
        void reboot(BootInfo* bootInfo);
        void shutdown(BootInfo* bootInfo);
};


