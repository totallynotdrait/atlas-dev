#pragma once

#include <liba/stdio.h>
#include <hardw/pci.h>

#include <data_structures/link.h>
#include <drivers/device/device.h>

void InitialiseUHCI(uint32_t id, PCI::PCIDeviceHeader* header);