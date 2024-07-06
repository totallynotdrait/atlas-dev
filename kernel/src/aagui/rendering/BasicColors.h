/* 
    BasicColors

    A basic header with classes containing the 16 web colors in different formats.

    HEX
    Decimal
 */

#pragma once
#include <stdint.h>

class HEXBasicColors {
    public:
        static const uint32_t 
        Black       = 0x000000,
        White       = 0xFFFFFF,
        Red         = 0xFF0000,
        Lime        = 0x00FF00,
        Blue        = 0x0000FF,
        Yellow      = 0xFFFF00,
        Cyan        = 0x00FFFF,
        Magenta     = 0xFF00FF,
        Silver      = 0xC0C0C0,
        Gray        = 0x808080,
        Maroon      = 0x800000,
        Olive       = 0x808000,
        Green       = 0x008000,
        Purple      = 0x800080,
        Teal        = 0x008080,
        Navy        = 0x000080;
};

class DBasicColors {
    public:
        static const uint32_t 
        Black       = 0,
        White       = 16777215,
        Red         = 16711680,
        Lime        = 65280,
        Blue        = 255,
        Yellow      = 16776960,
        Cyan        = 65535,
        Magenta     = 16711935,
        Silver      = 12632256,
        Gray        = 8421504,
        Maroon      = 8388608,
        Olive       = 8421376,
        Green       = 32768,
        Purple      = 8388736,
        Teal        = 32896,
        Navy        = 128;
};


extern HEXBasicColors hbc;
extern DBasicColors dbc;