#include "keyboard.h"
#include <liba/stdio.h>

bool isKeyboardEnabled = true;
bool isLeftShiftPressed;
bool isRightShiftPressed;
bool isNumpadEnabled = false;


char getch() {
    KeyboardInfo *key_info = (KeyboardInfo*)KEY_INFO_ADDRESS;
    while (!key_info->scancode) {
        __asm__ __volatile__("hlt");
    }

    uint8_t scancode = key_info->scancode;
    key_info->scancode = 0;

    // Handle key releases and ignore them
    if (scancode & 0x80) {
        return getch();
    }

    // Handle space key
    if (scancode == 0x39) {
        return ' ';
    }

    // Handle enter key
    if (scancode == 0x1C || scancode == 0xE01C) {
        return '\n';
    }

    // Handle backspace key
    if (scancode == 0x0E) {
        return '\b';
    }

    switch (scancode) {

        // arrow keys (ignores)
        case 0xE048: return getch();
        case 0xE04B: return getch();
        case 0xE050: return getch();
        case 0xE04D: return getch();

        // other keys
        case 0xE05C: return getch();
        case 0xE05D: return getch();
        case 0xE01D: return getch();
        case 0x1D5B: return getch();
        case 0xE038: return getch();
        case 0x0F: return getch();
        case 0x3A: return getch();

    }

    switch (scancode) {
        // keypad
        if (isNumpadEnabled) {
            case 0x52: return '0';
            case 0x4F: return '1';
            case 0x50: return '2';
            case 0x51: return '3';
            case 0x4B: return '4';
            case 0x4C: return '5';
            case 0x4D: return '6';
            case 0x47: return '7';
            case 0x48: return '8';
            case 0x49: return '9';
            case 0x37: return '*';
            case 0x4A: return '-';
            case 0x4E: return '+';
            case 0x53: return '.';
            case 0x35: return '/';       
        }
    }

    // Translate other scancodes to ASCII
    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);
    return ascii;
}



void HandleKeyboard(uint8_t scancode) {

    if (!isKeyboardEnabled) return;

    KeyboardInfo *key_info = (KeyboardInfo *)KEY_INFO_ADDRESS;

    
    switch (scancode) {
        case LeftShift:
            isLeftShiftPressed = true;
            key_info->shift = true;
            return;
        case LeftShift + 0x80:
            isLeftShiftPressed = false;
            key_info->shift = false;
            return;
        case RightShift:
            isRightShiftPressed = true;
            return;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            return;
        case LeftCTRL:
            key_info->ctrl = true;
            return;
        case LeftCTRL + 0x80:
            key_info->ctrl = false;
            return;
        case Enter:
            key_info->scancode = 0x1C;
            return;
        case BlocNum:
            isNumpadEnabled = true;
            return;
        case BlocNum + 0x80:
            isNumpadEnabled = false;
            return; 
        case Spacebar:
            key_info->scancode = 0x39;
            return;

        case Backspace:
            key_info->scancode = 0x0E;
            return;
    }

    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);
    key_info->scancode = scancode;
    /* if (ascii != 0) {
        AddCharacter(ascii);
        GKRenderer->putChar(ascii);
    } */
    
    /* printf("%c\n", getch());
    printf("ctrl: %d / shift: %d / key: %d / ASCII: %c\n", key_info->ctrl, key_info->shift, key_info->scancode, ascii); */
    
}