#pragma once
#include "../kata_renderer/kata_renderer.h"

class basesystem
{
    public:
    basesystem();
    static void broadcast(const char *message, const char *sender);
};


