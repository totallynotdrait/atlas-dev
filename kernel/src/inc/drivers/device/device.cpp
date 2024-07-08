#include "device.h"
#include <mem/memory.h>
#include <log.h>

Device* devices[MAX_DEVICE_COUNT];

void InitialiseDevices()
{
    memset(devices, 0, sizeof(Device*) * MAX_DEVICE_COUNT);
}

void RegisterDevice(Device* device)
{
    for(uint32_t i = 0; i < MAX_DEVICE_COUNT; i++)
    {
        if(devices[i] == nullptr)
        {
            devices[i] = device;
            return;
        }
    }

    log->failed("Too many devices added.");
    asm("hlt");
}

void UnregisterDevice(Device* device)
{
    for(uint32_t i = 0; i < MAX_DEVICE_COUNT; i++)
    {
        if(devices[i] == device)
        {
            devices[i] = nullptr;
            return;
        }
    }

    log->failed("Can't find the device.");
    asm("hlt");
}