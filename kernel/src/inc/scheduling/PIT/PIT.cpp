#include "PIT.h"
#include <IO/IO.h>
#include <kata_renderer/kata_renderer.h>

namespace PIT {
    double TimeSinceBoot = 0;
    uint16_t Divisor = 65535;

    void Sleepd(double seconds) {
        double startTime = TimeSinceBoot;
        while (TimeSinceBoot < startTime + seconds) {
            asm("hlt");
        }
        
    }

    uint8_t get_rtc_register(uint8_t reg)
    {
        outb(cmos_address, reg | 0x80);     // Disable NMI when sending register to read
        io_wait();                          // Small delay
        return inb(cmos_data);              // Return data at that register
    }

    void enable_rtc(void)
    {
        uint8_t prev_regB_value = get_rtc_register(0x0B);

        outb(cmos_address, 0x8B);                // Select register B again, because reading a CMOS register resets to register D
        io_wait();                               // Small delay
        outb(cmos_data, prev_regB_value | 0x40); // Set bit 6 to enable periodic interrupts at default rate of 1024hz

        get_rtc_register(0x0C);                  // Read status register C to clear out any pending IRQ8 interrupts
    }

    void disable_rtc(void)
    {
        uint8_t prev_regB_value;

        __asm__ __volatile__ ("cli");   

        prev_regB_value = get_rtc_register(0x0B);

        outb(cmos_address, 0x8B);                // Select register B again, because reading a CMOS register resets to register D
        io_wait();                               // Small delay
        outb(cmos_data, prev_regB_value & 0xBF); // Clear bit 6 to disable periodic interrupts

        __asm__ __volatile__ ("sti");   
    }

    void Sleep(uint64_t milliseconds) {
        Sleepd((double)milliseconds / 1000);
    }

    void SetDivisor(uint16_t divisor) {
        if (divisor < 100) divisor = 100;
        Divisor = divisor;
        outb(0x40, (uint8_t)(divisor & 0x00ff));
        io_wait();
        outb(0x40, (uint8_t)((divisor & 0xff00) >> 8));
    }

    uint64_t GetFrequency() {
        return BaseFrequency / Divisor;
    }

    void SetFrequency(uint64_t frequency) {
        SetDivisor(BaseFrequency / frequency);
    }

    void Tick() { 
        GKRenderer->paint();
        TimeSinceBoot += 1.0 / (double)GetFrequency();
    }

    void set_pit_channel_mode_frequency(const uint8_t channel, const uint8_t operating_mode, const uint16_t frequency) {
        // Invalid input
        if (channel > 2) return;

        __asm__ __volatile__ ("cli");

        /* PIT I/O Ports:
        * 0x40 - channel 0     (read/write) 
        * 0x41 - channel 1     (read/write) 
        * 0x42 - channel 2     (read/write) 
        * 0x43 - Mode/Command  (write only) 
        *
        * 0x43 Command register value bits (1 byte):
        * 7-6 select channel:
        *      00 = channel 0
        *      01 = channel 1
        *      10 = channel 2
        *      11 = read-back command
        * 5-4 access mode:
        *      00 = latch count value
        *      01 = lobyte only
        *      10 = hibyte only
        *      11 = lobyte & hibyte
        * 3-1 operating mode:
        *      000 = mode 0 (interrupt on terminal count)
        *      001 = mode 1 (hardware re-triggerable one-shot)
        *      010 = mode 2 (rate generator)
        *      011 = mode 3 (square wave generator)
        *      100 = mode 4 (software triggered strobe)
        *      101 = mode 5 (hardware triggered strobe)
        *      110 = mode 6 (rate generator, same as 010)
        *      111 = mode 7 (square wave generator, same as 011)
        * 0  BCD/Binary mode:
        *      0 = 16bit binary
        *      1 = 4-digit BCD (x86 does not use this!)
        */

        // Send the command byte, always sending lobyte/hibyte for access mode
        outb(0x43, (channel << 6) | (0x3 << 4) | (operating_mode << 1));

        // Send the frequency divider to the input channel
        outb(0x40 + channel, (uint8_t)frequency);           // Low byte
        outb(0x40 + channel, (uint8_t)(frequency >> 8));    // High byte

        __asm__ __volatile__ ("sti");
    }

}