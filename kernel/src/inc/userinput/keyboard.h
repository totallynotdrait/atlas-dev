#pragma once
#include "kbScancode.h"
#include <kata_renderer.h>

#define MAX_LENGTH 100

extern bool isKeyboardEnabled; // Declare the extern variable
extern char RetStr[MAX_LENGTH]; // Declare the extern variable

void HandleKeyboard(uint8_t scancode);
uint8_t getch();