#include "rtl8139.h"
#include <hardw/pci.h>
#include <drivers/serial/serial.h>
#include <liba/string.h>
#include <xxd.h>

PCI::pci_dev_t pci_rtl8139_device;
rtl8139_dev_t rtl8139_device;

uint32_t current_packet_ptr;

uint8_t TSAD_array[4] = {0x20, 0x24, 0x28, 0x2C};
uint8_t TSD_array[4] = {0x10, 0x14, 0x18, 0x1C};

