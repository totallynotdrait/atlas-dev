#include "ip.h"
#include <drivers/network/network_utils.h>
#include <liba/stdio.h>

uint8_t my_ip[] = {10, 0, 2, 14};
uint8_t test_target_ip[] = {10, 0, 2, 15};
uint8_t zero_hardware_addr[] = {0,0,0,0,0,0};

void get_ip_str(char * ip_str, uint8_t * ip) {
    printf(ip_str, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
}

uint16_t ip_calculate_checksum(ip_packet_t * packet) {
    // Treat the packet header as a 2-byte-integer array
    // Sum all integers up and flip all bits
    int array_size = sizeof(ip_packet_t) / 2;
    uint16_t * array = (uint16_t*)packet;
    uint8_t * array2 = (uint8_t*)packet;
    uint32_t sum = 0;
    for(int i = 0; i < array_size; i++) {
        sum += flip_short(array[i]);
    }
    uint32_t carry = sum >> 16;
    sum = sum & 0x0000ffff;
    sum = sum + carry;
    uint16_t ret = ~sum;
    return ret;
}