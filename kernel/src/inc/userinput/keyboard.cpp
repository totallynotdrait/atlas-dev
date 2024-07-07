#include "keyboard.h"
#include <liba/stdio.h>

char RetStr[MAX_LENGTH];
bool isKeyboardEnabled = true;
bool isLeftShiftPressed;
bool isRightShiftPressed;

void AddCharacter(char character) {
    int len = 0;
    while (RetStr[len] != '\0' && len < MAX_LENGTH - 1) {
        len++;
    }

    if (len < MAX_LENGTH - 1) {
        RetStr[len] = character;
        RetStr[len + 1] = '\0';
    }
}

void RemoveLastCharacter() {
    int len = 0;
    while (RetStr[len] != '\0' && len < MAX_LENGTH - 1) {
        len++;
    }

    if (len > 0) {
        RetStr[len - 1] = '\0';
    }
}

char getch() {
    KeyboardInfo *key_info = (KeyboardInfo *)KEY_INFO_ADDRESS;
    while (!key_info->scancode) {
        __asm__ __volatile__("hlt");
    }

    uint8_t scancode = key_info->scancode;
    key_info->scancode = 0;

    if (scancode & 0x80) {
        return getch();
    }
    
    if (scancode == 0x39) {
        return ' ';
    }

    if (scancode == 0x1C || scancode == 0xE01C) {
        return '\n';
    }

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
        case LeftCTRL + 0x80:
            key_info->ctrl = false;
        case Enter:
            key_info->scancode = 0x1C;
            return;

        case Spacebar:
            key_info->scancode = 0x39;
            return;

        case Backspace:
            GKRenderer->ClearChar();
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