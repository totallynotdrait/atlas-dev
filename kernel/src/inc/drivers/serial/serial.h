#include <liba/stdio.h>

#define PORT_COM1 0x3F8
#define PORT_COM2 0x2F8
#define PORT_COM3 0x3E8
#define PORT_COM4 0x2E8
#define PORT_COM5 0x5F8
#define PORT_COM6 0x4F8
#define PORT_COM7 0x5E8
#define PORT_COM8 0x4E8

void serial_init();
int serial_received();
char read_serial();
int is_transmit_empty();
void write_serial(char a);