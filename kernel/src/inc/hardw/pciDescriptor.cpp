#include <stdint.h>
#include <intnum/cstr.h>
#include <drivers/network/intel/e1000/e1000.h>

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
        switch (vendorID) {
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
            case 0x8086: // intel
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
                case 0x100E:
                    return "82540EM Gigabit Ethernet Controller";
                case 0x153A:
                    return "Ethernet Connection I217-LM";
                case 0x10EA:
                    return "82577LM Gigabit Network Connection";
            }
            case 0x10EC: // Realtek
                switch (deviceID) {
                case 0x8139:
                    return "RTL8139 Ethernet";

                }
            case 0x10DE: // NVIDIA
                switch (deviceID) {
                // rtx
                // 20s
                case 0x1F08:
                    return "RTX 2060";
                case 0x1F06:
                    return "RTX 2060 Super";
                case 0x1F02:
                    return "RTX 2070";
                case 0x1F07:
                    return "RTX 2070 Super";
                case 0x1E82:
                    return "RTX 2080";
                case 0x1E81:
                    return "RTX 2080 Super";
                case 0x1E07:
                    return "RTX 2080 Ti";

                // 30s

                case 0x2504:
                    return "RTX 3060";
                case 0x2486:
                    return "RTX 3060 Ti";
                case 0x2484:
                    return "RTX 3070";
                case 0x2488:
                    return "RTX 3070 Ti";
                case 0x2206:
                    return "RTX 3080";
                case 0x2208:
                    return "RTX 4080 Ti";
                case 0x2204:
                    return "RTX 2090";
                
                // 40s
                case 0x2864:
                    return "RTX 4060";
                case 0x2865:
                    return "RTX 4060 Ti";
                case 0x2866:
                    return "RTX 4070";
                case 0x2867:
                    return "RTX 4070 Ti";
                case 0x2868:
                    return "RTX 4080";
                case 0x2869:
                    return "RTX 4090";
                }
        }
        return to_hexstring(deviceID);
    }

    const char* MassStorageControllerSubclassName(uint8_t subclassCode) {
        switch (subclassCode) {
            case 0x00:
                return "SCSI Bus Controller";
            case 0x01:
                return "IDE Controller";
            case 0x02:
                return "Floppy Disk Controller"; 
            case 0x03:
                return "IPI Bus Controller";
            case 0x04:
                return "RAID Controller";
            case 0x05:
                return "ATA Controller";
            case 0x06:
                return "Serial ATA Controller";
            case 0x07:
                return "Serial Attached SCSI Controller";
            case 0x08:
                return "Non-Volatile Memory Controller";
            case 0x80:
                return "Not known (Other)";
        }
        return to_hexstring(subclassCode);
    }

    const char* GetSubclassName(uint8_t classCode, uint8_t subclassCode) {
        switch (classCode) {
            case 0x01: // Mass Storage Controller
                return MassStorageControllerSubclassName(subclassCode);
        }
        return to_hexstring(subclassCode);
    }

    const char* GetProgIFName(uint8_t classCode, uint8_t subclassCode, uint8_t progIF){
        switch (classCode) {
            case 0x01:
                switch (subclassCode) {
                    case 0x01: // IDE Controller
                        switch (progIF) {
                            case 0x00: return "ISA Compatibility mode-only controller";
                            case 0x05: return "PCI native mode-only controller";
                            case 0x0A: return "ISA Compatibility mode controller";
                            case 0x0F: return "PCI native mode controller";
                            case 0x80: return "ISA Compatibility mode-only controller";
                            case 0x85: return "PCI native mode-only controller, supports bus mastering";
                            case 0x8A: return "ISA Compatibility mode controller";
                            case 0x8F: return "PCI native mode controller";
                        }

                    case 0x05: // ATA Controller 
                        switch (progIF) {
                            case 0x20: return "Single DMA";
                            case 0x30: return "Chained DMA";
                        }
                    case 0x06: // Serial ATA Controller 
                        switch (progIF) {
                            case 0x00: return "Vendor Specific Interface";
                            case 0x01: return "AHCI 1.0";
                            case 0x02: return "Serial Storage Bus";
                        }
                    case 0x07: // Serial Attached SCSI Controller
                        switch (progIF) {
                            case 0x00: return "SAS";
                            case 0x01: return "Serial Storage Bus";
                        }
                    case 0x08: // Non-Volatile Memory Controller
                        switch (progIF) {
                            case 0x00: return "NVMHCI";
                            case 0x01: return "NVM Express";
                        }
                }

            case 0x03:
                switch (subclassCode) {
                    case 0x00:
                        switch (progIF) {
                            case 0x00: return "VGA Controller";
                            case 0x01: return "8514-Compatible Controller";
                        }
                }

            case 0x0C:
                switch (subclassCode) {
                    case 0x03:
                        switch (progIF) {
                            case 0x00: return "UHCI Controller";                                
                            case 0x10: return "OHCI Controller";                               
                            case 0x20: return "EHCI (USB2) Controller";                                
                            case 0x30: return "XHCI (USB3) Controller";    
                            case 0x80: return "Unspecified";
                            case 0xFE: return "USB Device (Not a Host Controller)";  
                        }
                }    
        }
        return to_hexstring(progIF);
    }
} 
