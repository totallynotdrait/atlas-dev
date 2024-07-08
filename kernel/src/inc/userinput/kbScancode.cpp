#include "kbScancode.h"
#include <liba/string.h>
#include <log.h>

namespace QWERTYKeyboard {
    const char ASCIITable[] = {
         0 ,  0 , '1', '2',
        '3', '4', '5', '6',
        '7', '8', '9', '0',
        '-', '=',  0 ,  0 ,
        'q', 'w', 'e', 'r',
        't', 'y', 'u', 'i',
        'o', 'p', '[', ']',
         0 ,  0 , 'a', 's',
        'd', 'f', 'g', 'h',
        'j', 'k', 'l', ';',
        '\'','`',  0 , '\\',
        'z', 'x', 'c', 'v',
        'b', 'n', 'm', ',',
        '.', '/',  0 , '*',
         0 , ' '
    };

    void loadKeys(const char* keyMap) {
        if (keyMap == "it-IT") {
            const char ASCIITable[] = {
                0 ,  0 , '1', '2',
                '3', '4', '5', '6',
                '7', '8', '9', '0',
                '\'', 'i',  0 ,  0 ,
                'q', 'w', 'e', 'r',
                't', 'y', 'u', 'i',
                'o', 'p', 'e', '+',
                0 ,  0 , 'a', 's',
                'd', 'f', 'g', 'h',
                'j', 'k', 'l', 'o',
                'a', 'u',  0 , '\\',
                'z', 'x', 'c', 'v',
                'b', 'n', 'm', ',',
                '.', '-',  0 , '*',
                0 , ' '
            };
            log->ok("Loaded it-IT keyMap.");
        } else if (keyMap == "en-US") {
            const char ASCIITable[] = {
                0 ,  0 , '1', '2',
                '3', '4', '5', '6',
                '7', '8', '9', '0',
                '-', '=',  0 ,  0 ,
                'q', 'w', 'e', 'r',
                't', 'y', 'u', 'i',
                'o', 'p', '[', ']',
                0 ,  0 , 'a', 's',
                'd', 'f', 'g', 'h',
                'j', 'k', 'l', ';',
                '\'','`',  0 , '\\',
                'z', 'x', 'c', 'v',
                'b', 'n', 'm', ',',
                '.', '/',  0 , '*',
                0 , ' '
            };
            log->ok("Loaded en-US keyMap.");
        } else {
            log->failed("keyMap not recognized.");
        }
    }

    char Translate(uint8_t scancode, bool uppercase) {
        if (scancode == 28) return '\n';
        if (scancode > 58) return 0;

        if (uppercase) {
            return ASCIITable[scancode] - 32;
        } else return ASCIITable[scancode];
    }

}