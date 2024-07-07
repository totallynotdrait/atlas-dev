/* 
    Atlas Development Project

    A similar implementation of stdio.h from the C library

 */


#include "stdio.h"

void printf(const char* format, ...) {
    va_list arg;
    va_start(arg, format);
    char* chr = (char*) format;
    while(*chr != 0) {
        if (*chr == '%') {
            chr++;
            switch(*chr) {
                case 'c': {
                    char val = va_arg(arg, int);
                    GKRenderer->putChar(val, GKRenderer->CursorPosition.X, GKRenderer->CursorPosition.Y);
                    break;
                }
                case 's': {
                    char* val = va_arg(arg, char*);
                    while(*val) {
                        GKRenderer->putChar(*val++, GKRenderer->CursorPosition.X, GKRenderer->CursorPosition.Y);
                        GKRenderer->CursorPosition.X += 8;
                    }
                    break;
                }
                case 'd': {
                    uint64_t val = va_arg(arg, int);
                    const char* strVal = to_string(val);
                    while(*strVal) {
                        GKRenderer->putChar(*strVal++, GKRenderer->CursorPosition.X, GKRenderer->CursorPosition.Y);
                        GKRenderer->CursorPosition.X += 8;
                    }
                    break;
                }
                case 'x': {
                    uint64_t val = va_arg(arg, int);
                    const char* strVal = to_hexstring(val);
                    while(*strVal) {
                        GKRenderer->putChar(*strVal++, GKRenderer->CursorPosition.X, GKRenderer->CursorPosition.Y);
                        GKRenderer->CursorPosition.X += 8;
                    }
                    break;
                }
            }
        } else if (*chr == '\\') {
            chr++;
            if (*chr == 'x') {
                chr++;
                int newY = *chr - '0';  // Convert ASCII digit to integer
                chr++;
                int newX = *chr - '0';  // Convert ASCII digit to integer
                GKRenderer->CursorPosition.X = newX * 8;
                GKRenderer->CursorPosition.Y = newY * 16;
            } else if (*chr == 'c') {
                GKRenderer->Clear();
            }
        } else if (*chr == '\n') {
            GKRenderer->Next();
            GKRenderer->CursorPosition.X = 0; 
        } else if (*chr == '\t') {
            GKRenderer->CursorPosition.X += 4 * 8;
        } else {
            GKRenderer->putChar(*chr, GKRenderer->CursorPosition.X, GKRenderer->CursorPosition.Y);
            GKRenderer->CursorPosition.X+=8;
        } 
        if (GKRenderer->CursorPosition.X + 8 > GKRenderer->TargetFramebuffer->Width) {
            GKRenderer->CursorPosition.X = 0;
            GKRenderer->CursorPosition.Y += 16;
        }
        chr++;
    }
    va_end(arg);
}

void scanf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    // Print the format string
    const char* fmt_ptr = format;
    while (*fmt_ptr != '\0') {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            if (*fmt_ptr == 'c' || *fmt_ptr == 's' || *fmt_ptr == 'd' || *fmt_ptr == 'x') {
                fmt_ptr++;  // Skip the format specifier
                continue;
            }
        }
        GKRenderer->putChar(*fmt_ptr, GKRenderer->CursorPosition.X, GKRenderer->CursorPosition.Y);
        GKRenderer->CursorPosition.X += 8;
        fmt_ptr++;
    }

    isKeyboardEnabled = true;
    char buffer[256];
    char* buffer_ptr = buffer;
    char c;

    // Read input from the keyboard
    while ((c = getch()) != '\n') {
        *buffer_ptr++ = c;
        GKRenderer->putChar(c);
    }
    *buffer_ptr = '\0'; // Null terminate the input string

    // Reset the buffer pointer for parsing
    buffer_ptr = buffer;

    // Parse the format string and extract input values
    fmt_ptr = format;
    while (*fmt_ptr != '\0') {
        if (*fmt_ptr == '%') {
            fmt_ptr++;
            switch (*fmt_ptr) {
                case 'c': {
                    char* char_ptr = va_arg(args, char*);
                    *char_ptr = *buffer_ptr++;
                    break;
                }
                case 's': {
                    char* str_ptr = va_arg(args, char*);
                    while (*buffer_ptr != '\0') {
                        *str_ptr++ = *buffer_ptr++;
                    }
                    *str_ptr = '\0';  // Null terminate the string
                    break;
                }
                // Add other format specifiers as needed
            }
        }
        fmt_ptr++;
    }

    va_end(args);
}