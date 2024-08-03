#pragma once

#include <drivers/device/device.h>
#include <operators.h>

class ATADevice : public Device
{
public:
    ATADevice() {}
    ~ATADevice() {}

    DeviceType GetType() { return DeviceType::ATA_PIO; };

    void Read(uint64_t LBA, void* buffer, uint64_t size);
    void Write(uint64_t LBA, void* buffer, uint64_t size);
};