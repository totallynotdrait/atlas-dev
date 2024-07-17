#pragma once
#include <stdint.h>

namespace QWERTYKeyboard {

    #define LeftCTRL 0x1D
    #define LeftShift 0x2A
    #define RightShift 0x36
    #define Enter 0x1C
    #define Backspace 0x0E
    #define Spacebar 0x39
    #define BlocNum 0x45

    extern const char* loadedKeys;
    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}