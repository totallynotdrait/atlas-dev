#include "xxd.h"
#include <drivers/serial/serial.h>

#define DUMP_COLS 16  // Ensure DUMP_COLS is defined

void xxd(void *data, unsigned int len) {
    unsigned int i, j;

    for (i = 0; i < len + ((len % DUMP_COLS) ? (DUMP_COLS - len % DUMP_COLS) : 0); i++) {
        /* print offset */
        if (i % DUMP_COLS == 0) {
            printf("0x%x: ", i);  // Print offset with 8 digits for better alignment
        }

        /* print hex data */
        if (i < len) {
            printf("0x%x ", 0xFF & ((char*)data)[i]);  // Print hex with leading zeros
        } else {
            printf("   ");  // Aligning for ASCII dump
        }

        /* print ASCII dump */
        if (i % DUMP_COLS == (DUMP_COLS - 1)) {
            printf(" |");
            for (j = i - (DUMP_COLS - 1); j <= i; j++) {
                if (j >= len) {
                    write_serial(' ');
                } else {
                    char ch = 0xFF & ((char*)data)[j];
                    if (ch >= 32 && ch <= 126) {
                        write_serial(ch);  // Printable ASCII characters
                    } else {
                        write_serial('.');  // Non-printable characters are shown as '.'
                    }
                }
            }
            printf("|\n");
        }
    }
}
