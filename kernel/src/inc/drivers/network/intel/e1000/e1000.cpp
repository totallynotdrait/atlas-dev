#include "e1000_base_address.h"
#include "e1000.h"

class MMIOUtils
{
    public:
        static uint8_t read8 (uint64_t p_address);
        static uint16_t read16 (uint64_t p_address);
        static uint32_t read32 (uint64_t p_address);
        static uint64_t read64 (uint64_t p_address);
        static void write8 (uint64_t p_address,uint8_t p_value);
        static void write16 (uint64_t p_address,uint16_t p_value);
        static void write32 (uint64_t p_address,uint32_t p_value);
        static void write64 (uint64_t p_address,uint64_t p_value);
};


uint8_t MMIOUtils::read8(uint64_t p_address) {
    return *((volatile uint8_t*)(p_address));
}

uint16_t MMIOUtils::read16(uint64_t p_address) {
    return *((volatile uint16_t*)(p_address));
    
}

uint32_t MMIOUtils::read32(uint64_t p_address) {
    return *((volatile uint32_t*)(p_address));
    
}

uint64_t MMIOUtils::read64(uint64_t p_address) {
    return *((volatile uint64_t*)(p_address));    
}

void MMIOUtils::write8(uint64_t p_address,uint8_t p_value) {
    (*((volatile uint8_t*)(p_address)))=(p_value);
}

void MMIOUtils::write16(uint64_t p_address,uint16_t p_value) {
    (*((volatile uint16_t*)(p_address)))=(p_value);    
}

void MMIOUtils::write32(uint64_t p_address,uint32_t p_value) {
    (*((volatile uint32_t*)(p_address)))=(p_value);
    
}

void MMIOUtils::write64(uint64_t p_address,uint64_t p_value)
{
    (*((volatile uint64_t*)(p_address)))=(p_value);    
}


void E1000::writeCommand( uint16_t p_address, uint32_t p_value)
{
    if ( bar_type == 0 )
    {
        MMIOUtils::write32(mem_base+p_address,p_value);
    }
    else
    {
        Ports::outportl(io_base, p_address);
        Ports::outportl(io_base + 4, p_value);
    }
}

uint32_t E1000::readCommand( uint16_t p_address)
{
    if ( bar_type == 0 )
    {
        return MMIOUtils::read32(mem_base+p_address);
    }
    else
    {
        Ports::outportl(io_base, p_address);
        return Ports::inportl(io_base + 4);
    }
}

bool E1000::detectEEProm()
{
    uint32_t val = 0;
    writeCommand(REG_EEPROM, 0x1); 

    for(int i = 0; i < 1000 && ! eerprom_exists; i++)
    {
            val = readCommand( REG_EEPROM);
            if(val & 0x10) {
                log->ok("eeprom is true");
                eerprom_exists = true;
            
            } else {
                log->failed("eeprom is false");
                eerprom_exists = false;
            }
    }
    return eerprom_exists;
}

uint32_t E1000::eepromRead( uint8_t addr)
{
	uint16_t data = 0;
	uint32_t tmp = 0;
        if ( eerprom_exists)
        {
            	writeCommand( REG_EEPROM, (1) | ((uint32_t)(addr) << 8) );
        	while( !((tmp = readCommand(REG_EEPROM)) & (1 << 4)) );
        }
        else
        {
            writeCommand( REG_EEPROM, (1) | ((uint32_t)(addr) << 2) );
            while( !((tmp = readCommand(REG_EEPROM)) & (1 << 1)) );
        }
	data = (uint16_t)((tmp >> 16) & 0xFFFF);
	return data;
}

bool E1000::readMACAddress()
{
    log->print("Checking eerprom existence...");
    if ( eerprom_exists)
    {
        uint32_t temp;
        log->info("0");
        temp = eepromRead( 0);
        log->info("0.1");
        mac[0] = temp &0xff;
        log->info("0.2");
        mac[1] = temp >> 8;
        log->info("0.3");
        log->info("1");
        temp = eepromRead( 1);
        mac[2] = temp &0xff;
        mac[3] = temp >> 8;
        log->info("2");
        temp = eepromRead( 2);
        mac[4] = temp &0xff;
        mac[5] = temp >> 8;
        log->ok("Eerprom existence checked.");
    }
    else
    {
        log->failed("Failed checking, setting...");
        uint8_t * mem_base_mac_8 = (uint8_t *) (mem_base+0x5400);
        uint32_t * mem_base_mac_32 = (uint32_t *) (mem_base+0x5400);
        log->print("Start at first loop...");
        if ( mem_base_mac_32[0] != 0 )
        {
            for(int i = 0; i < 6; i++)
            {
                log->info("%d: mac", i);
                mac[i] = mem_base_mac_8[i];
            }
        }
        else return false;
    }
    return true;
}

void E1000::rxinit()
{
    uint8_t * ptr;
    struct e1000_rx_desc *descs;

    // Allocate buffer for receive descriptors.
    ptr = (uint8_t *)(malloc(sizeof(struct e1000_rx_desc) * E1000_NUM_RX_DESC + 16));

    descs = (struct e1000_rx_desc *)ptr;
    for (int i = 0; i < E1000_NUM_RX_DESC; i++)
    {
        rx_descs[i] = (struct e1000_rx_desc *)((uint8_t *)descs + i * 16);
        rx_descs[i]->addr = (uint64_t)(uint8_t *)(malloc(8192 + 16));
        rx_descs[i]->status = 0;
    }

    writeCommand(REG_TXDESCLO, (uint32_t)((uint64_t)ptr >> 32));
    writeCommand(REG_TXDESCHI, (uint32_t)((uint64_t)ptr & 0xFFFFFFFF));

    writeCommand(REG_RXDESCLO, (uint64_t)ptr);
    writeCommand(REG_RXDESCHI, 0);

    writeCommand(REG_RXDESCLEN, E1000_NUM_RX_DESC * 16);

    writeCommand(REG_RXDESCHEAD, 0);
    writeCommand(REG_RXDESCTAIL, E1000_NUM_RX_DESC - 1);
    rx_cur = 0;
    writeCommand(REG_RCTRL, RCTL_EN | RCTL_SBP | RCTL_UPE | RCTL_MPE | RCTL_LBM_NONE | RTCL_RDMTS_HALF | RCTL_BAM | RCTL_SECRC | RCTL_BSIZE_8192);
}

void E1000::txinit()
{
    uint8_t * ptr;
    struct e1000_tx_desc *descs;

    // Allocate buffer for transmit descriptors.
    ptr = (uint8_t *)(malloc(sizeof(struct e1000_tx_desc) * E1000_NUM_TX_DESC + 16));

    descs = (struct e1000_tx_desc *)ptr;
    for (int i = 0; i < E1000_NUM_TX_DESC; i++)
    {
        tx_descs[i] = (struct e1000_tx_desc *)((uint8_t *)descs + i * 16);
        tx_descs[i]->addr = 0;
        tx_descs[i]->cmd = 0;
        tx_descs[i]->status = TSTA_DD;
    }

    writeCommand(REG_TXDESCHI, (uint32_t)((uint64_t)ptr >> 32));
    writeCommand(REG_TXDESCLO, (uint32_t)((uint64_t)ptr & 0xFFFFFFFF));

    // Now setup total length of descriptors.
    writeCommand(REG_TXDESCLEN, E1000_NUM_TX_DESC * 16);

    // Setup numbers.
    writeCommand(REG_TXDESCHEAD, 0);
    writeCommand(REG_TXDESCTAIL, 0);
    tx_cur = 0;
    writeCommand(REG_TCTRL, TCTL_EN
                            | TCTL_PSP
                            | (15 << TCTL_CT_SHIFT)
                            | (64 << TCTL_COLD_SHIFT)
                            | TCTL_RTLC);

    // This line of code overrides the one before it but I left both to highlight that the previous one works with e1000 cards, but for the e1000e cards
    // you should set the TCTRL register as follows. For detailed description of each bit, please refer to the Intel Manual.
    // In the case of I217 and 82577LM packets will not be sent if the TCTRL is not configured using the following bits.
    writeCommand(REG_TCTRL, 0b0110000000000111111000011111010);
    writeCommand(REG_TIPG, 0x0060200A);
}



void E1000::enableInterrupt()
{
    writeCommand(REG_IMASK ,0x1F6DC);
    writeCommand(REG_IMASK ,0xff & ~4);
    readCommand(0xc0);

}



void E1000::handleReceive()
{
    uint16_t old_cur;
    bool got_packet = false;
 
    while((rx_descs[rx_cur]->status & 0x1))
    {
            got_packet = true;
            uint8_t *buf = (uint8_t *)rx_descs[rx_cur]->addr;
            uint16_t len = rx_descs[rx_cur]->length;

            // Here you should inject the received packet into your network stack


            rx_descs[rx_cur]->status = 0;
            old_cur = rx_cur;
            rx_cur = (rx_cur + 1) % E1000_NUM_RX_DESC;
            writeCommand(REG_RXDESCTAIL, old_cur );
    }    
}

int E1000::sendPacket(const void * p_data, uint16_t p_len)
{    
    tx_descs[tx_cur]->addr = (uint64_t)p_data;
    tx_descs[tx_cur]->length = p_len;
    tx_descs[tx_cur]->cmd = CMD_EOP | CMD_IFCS | CMD_RS;
    tx_descs[tx_cur]->status = 0;
    uint8_t old_cur = tx_cur;
    tx_cur = (tx_cur + 1) % E1000_NUM_TX_DESC;
    writeCommand(REG_TXDESCTAIL, tx_cur);   
    while(!(tx_descs[old_cur]->status & 0xff));    
    return 0;
}

void E1000::startLink()
{
    // Perform a software reset of the device
    writeCommand(REG_CTRL, CTRL_RST);
    while (readCommand(REG_CTRL) & CTRL_RST);

    // Configure the Device Control Register (CTRL)
    uint32_t ctrl = readCommand(REG_CTRL);
    ctrl |= CTRL_SLU | CTRL_ASDE; // Set Link Up and Auto-Speed Detection Enable
    ctrl &= ~CTRL_LRST;           // Clear the Link Reset
    ctrl |= CTRL_RFCE | CTRL_TFCE; // Enable flow control
    ctrl &= ~(CTRL_PHY_RST);      // Clear PHY Reset
    writeCommand(REG_CTRL, ctrl);

    // Configure the Receive Control Register (RCTL)
    uint32_t rctl = readCommand(REG_RCTL);
    rctl |= RCTL_EN;              // Enable receiver
    rctl |= RCTL_BAM;             // Broadcast Accept Mode
    rctl &= ~RCTL_LPE;            // Clear Long Packet Enable
    writeCommand(REG_RCTL, rctl);

    // Configure the Transmit Control Register (TCTL)
    uint32_t tctl = readCommand(REG_TCTL);
    tctl |= TCTL_EN;              // Enable transmitter
    tctl |= TCTL_PSP;             // Pad Short Packets
    tctl |= (15 << TCTL_CT_SHIFT); // Collision Threshold
    tctl |= (64 << TCTL_COLD_SHIFT); // Collision Distance
    writeCommand(REG_TCTL, tctl);

    // Configure the Inter-Packet Gap (TIPG)
    writeCommand(REG_TIPG, 0x0060200A);
}

void E1000::fire()
{
    writeCommand(REG_IMASK, 0x1);
    
    uint32_t status = readCommand(0xc0);
    if(status & 0x04)
    {
        startLink();
    }
    else if(status & 0x10)
    {
        // good threshold
    }
    else if(status & 0x80)
    {
        handleReceive();
    }
}

bool E1000::start()
{
    log->print("Starting E1000 Ethernet drivers...");
    log->print("Detecting EEProm...");
    detectEEProm();
    log->print("Reading MAC Address...");
    if (!readMACAddress()) {
        log->failed("Failed at reading MAC Address.");
        return false;
    }

    log->print("Starting link...");
    startLink();
    
    log->print("Writting commands...");
    for(int i = 0; i < 0x80; i++)
        writeCommand(0x5200 + i*4, 0);

    log->print("Enabling interrupt...");
    enableInterrupt();

    log->print("Starting rxinit...");
    rxinit();
    log->print("Starting txinit...");
    txinit();        
    log->ok("Ethernet driver started successfully");
    return true;
}