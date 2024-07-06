#include "keyboard.h"

char RetStr[MAX_LENGTH]; // Declare the global variable
bool isKeyboardEnabled = false; // Declare the global variable
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

uint8_t getch() {
    return inb(0x60);
} 

void HandleKeyboard(uint8_t scancode) {

    if (!isKeyboardEnabled) return;

    switch (scancode) {
        case LeftShift:
            isLeftShiftPressed = true;
            return;
        case LeftShift + 0x80:
            isLeftShiftPressed = false;
            return;
        
        case RightShift:
            isRightShiftPressed = true;
            return;
        case RightShift + 0x80:
            isRightShiftPressed = false;
            return;

        case Enter:
            GKRenderer->Next();
            return;

        case Spacebar:
            AddCharacter(' ');
            return;

        case Backspace:
            RemoveLastCharacter();
            return;
    }

    char ascii = QWERTYKeyboard::Translate(scancode, isLeftShiftPressed | isRightShiftPressed);

    if (ascii != 0) {
        AddCharacter(ascii);
        GKRenderer->putChar(ascii);
    }
}