#pragma once
#include <stdint.h>

namespace PIT {
    extern double TimeSinceBoot;
    const uint64_t BaseFrequency = 1193182;

    void Sleepd(double seconds);
    void Sleep(uint64_t milliseconds);

    void SetDivisor(uint16_t divisor);
    uint64_t GetFrequency();
    void SetFrequency(uint64_t frequency);
    void Tick();
    void set_pit_channel_mode_frequency(const uint8_t channel, const uint8_t operating_mode, const uint16_t frequency);
}