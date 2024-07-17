#pragma once
#include "kbScancode.h"
#include <kata_renderer.h>
#include <inc/global_addresses.h>

struct KeyboardInfo {
    uint8_t scancode;
    bool    shift;
    bool    ctrl;
} __attribute__ ((packed));

extern bool isKeyboardEnabled; // Declare the extern variable

void HandleKeyboard(uint8_t scancode);
char getch();