/* 
    Intel Basic Network Drivers
    (i hate broadcom)

    This driver implements the usage of the Intel e1000, I217 and 82577LM Network (probably ethernet) drivers to do basic stuff, you know?

    Huge shout out to osdev wiki!

    sources;
    https://wiki.osdev.org/Intel_Ethernet_i217
*/

#pragma once
#include <stdint.h>
#include <hardw/pci.h>
#include <interrupts/interrupts.h>
#include <mem/heap.h>
#include "MMIO/MMIO.h"

#define E1000_NUM_RX_DESC 32
#define E1000_NUM_TX_DESC 8

struct e1000_rx_desc {
        volatile uint64_t addr;
        volatile uint16_t length;
        volatile uint16_t checksum;
        volatile uint8_t status;
        volatile uint8_t errors;
        volatile uint16_t special;
} __attribute__((packed));

struct e1000_tx_desc {
        volatile uint64_t addr;
        volatile uint16_t length;
        volatile uint8_t cso;
        volatile uint8_t cmd;
        volatile uint8_t status;
        volatile uint8_t css;
        volatile uint16_t special;
} __attribute__((packed));


class E1000 {
    private:
        
        uint8_t bar_type;     // Type of BAR0
        uint16_t io_base;     // IO Base Address
        uint64_t  mem_base;   // MMIO Base Address
        bool eerprom_exists;  // A flag indicating if eeprom exists
        uint8_t mac [6];      // A buffer for storing the mack address
        struct e1000_rx_desc *rx_descs[E1000_NUM_RX_DESC]; // Receive Descriptor Buffers
        struct e1000_tx_desc *tx_descs[E1000_NUM_TX_DESC]; // Transmit Descriptor Buffers
        uint16_t rx_cur;      // Current Receive Descriptor Buffer
        uint16_t tx_cur;      // Current Transmit Descriptor Buffer
        
        
        // Send Commands and read results From NICs either using MMIO or IO Ports
        void writeCommand( uint16_t p_address, uint32_t p_value);
        uint32_t readCommand(uint16_t p_address);


        bool detectEEProm(); // Return true if EEProm exist, else it returns false and set the eerprom_existsdata member
        uint32_t eepromRead( uint8_t addr); // Read 4 bytes from a specific EEProm Address
        bool readMACAddress();       // Read MAC Address
        //void startLink ();           // Start up the network
        void rxinit();               // Initialize receive descriptors an buffers
        void txinit();               // Initialize transmit descriptors an buffers
        void enableInterrupt();      // Enable Interrupts
        void handleReceive();        // Handle a packet reception.
    public:

        //E1000(PCI::PCIDeviceHeader * _pciConfigHeader); // Constructor. takes as a parameter a pointer to an object that encapsulate all he PCI configuration data of the device
        void start ();                             // perform initialization tasks and starts the driver
        void fire (interrupt_frame * p_interruptContext);  // This method should be called by the interrupt handler 
        uint8_t * getMacAddress ();                         // Returns the MAC address
        int sendPacket(const void * p_data, uint16_t p_len);  // Send a packet
        //~E1000();                                             // Default Destructor
};