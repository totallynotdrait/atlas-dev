#pragma once
#include <stdint.h>

#define USB_STRING_SIZE                 127

// USB Speeds
#define USB_FULL_SPEED                  0x00 // up to 5 gb/s
#define USB_LOW_SPEED                   0x01 // up to 1.5 mb/s
#define USB_HIGH_SPEED                  0x02 // up to 480 mb/s

class Controller
{
public:
    Controller() {}
    virtual ~Controller();

    virtual void Poll() = 0;
};