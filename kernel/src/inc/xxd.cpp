#include "xxd.h"
#include <drivers/serial/serial.h>


void xxd(void * data, unsigned int len)
{
    unsigned int i, j;

    for(i = 0; i < len + ((len % DUMP_COLS) ? (DUMP_COLS - len % DUMP_COLS) : 0); i++) {
        /* print offset */
        if(i % DUMP_COLS == 0) {
            log->info("0x%x: ", i);
        }

        /* print hex data */
        if(i < len) {
            log->info("%x ", 0xFF & ((char*)data)[i]);
        }
        else {
            /* end of block, just aligning for ASCII dump */
            log->info("   ");
        }

        /* print ASCII dump */
        if(i % DUMP_COLS == (DUMP_COLS - 1)) {
            for(j = i - (DUMP_COLS - 1); j <= i; j++) {
                if(j >= len) {
                    write_serial(' ');
                }
                write_serial(0xFF & ((char*)data)[j]);
            }
            write_serial('\n');
        }
    }
}