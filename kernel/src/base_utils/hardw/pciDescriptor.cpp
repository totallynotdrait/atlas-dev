#include <stdint.h>
#include "../intnum/cstr.h"

namespace PCI {
    const char* DeviceClasses[] {
        "Unclassified",
        "Mass Storage Controller",
        "Network Controller",
        "Display Controller",
        "Multimedia Controller",
        "Memory Controller",
        "Bridge",
        "Simple Communication Controller",
        "Base System Peripheral",
        "Input Device Controller",
        "Docking Station",
        "Processor",
        "Serial Bus Controller",
        "Wireless Controller",
        "Intelligent Controller",
        "Satellite Communication Controller",
        "Encryption Controller",
        "Signal Processing Controller",
        "Processing Accelerator",
        "Non-Essential Instrumentation",
    };

    const char* GetVendorName(uint16_t vendorID) {
        switch (vendorID)
        {
        case 0x8086:
            return "Intel Corporation";
        
        case 0x1022:
            return "AMD";
        case 0x10DE:
            return "NVIDIA Corporation";
        case 0x10EC:
            return "Realtek Semiconductor Corporation";
        case 0x15B7:
            return "Sandisk Corporation";
        case 0x1234:
            return "Brain Actuated Technologies";
        }
        return to_hexstring(vendorID);
    }

    const char* GetDeviceName(uint16_t vendorID, uint16_t deviceID) {
        switch (vendorID) {
            case 0x8086:
            switch (deviceID) {
                case 0x29C0:
                    return "Express DRAM Controller";
                case 0x2918:
                    return "LPC Interface Controller";
                case 0x2922:
                    return "6 port SATA Controller [AHCI mode]";
                case 0x2930:
                    return "SMBus Controller";
                case 0x43ED:
                    return "Tiger Lake-H USB 3.2 Gen 2x1 xHCI Host Controller";
                case 0x43EF:
                    return "Tiger Lake-H Shared SRAM";
                case 0x43E8:
                    return "Tiger Lake-H Serial IO I2C Controller #0";
                case 0x43E0:
                    return "Tiger Lake-H Management Engine Interface";
                case 0x2822:
                    return "SATA Controller [RAID mode]";
                case 0x43B0:
                    return "Tiger Lake-H PCI Express Root Port #9";
                case 0x4386:
                    return "H570 LPC/eSPI Controller";
                case 0x43A3:
                    return "Tiger Lake-H SMBus Controller";
                case 0x43A4:
                    return "Tiger Lake-H SPI Controller";
            }
        }
        return to_hexstring(deviceID);

        // https://www.youtube.com/watch?v=GxLL_lbOPo4&list=PLxN4E629pPnJxCQCLy7E0SQY_zuumOVyZ&index=20&ab_channel=Poncho
        // (11:05)
    }
} 
