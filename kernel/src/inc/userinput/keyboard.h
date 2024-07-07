#pragma once
#include "kbScancode.h"
#include <kata_renderer.h>
#include <inc/global_addresses.h>

#define MAX_LENGTH 100

struct KeyboardInfo {
    uint8_t scancode;
    bool    shift;
    bool    ctrl;
} __attribute__ ((packed));

extern bool isKeyboardEnabled; // Declare the extern variable
extern char RetStr[MAX_LENGTH]; // Declare the extern variable

void HandleKeyboard(uint8_t scancode);
char getch();