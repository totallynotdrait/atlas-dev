#pragma once
#include <stdint.h>

#define USB_STRING_SIZE                 127

// USB Speeds
#define USB_FULL_SPEED                  0x00
#define USB_LOW_SPEED                   0x01
#define USB_HIGH_SPEED                  0x02

class Controller
{
public:
    Controller() {}
    virtual ~Controller();

    virtual void Poll() = 0;
};