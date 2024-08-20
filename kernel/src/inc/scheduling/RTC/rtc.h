#pragma once

#include <stdint.h>
#include <log.h>



struct tm
{
    uint32_t year;
    uint32_t month;
    uint32_t day;
    uint32_t hour;
    uint32_t minute;
    uint32_t second;
};

extern tm *timedate;

void InitRTC(struct tm* kernel_time);
void GetTimeRTC(struct tm* tm);
uint8_t BCDToBinary(uint8_t bcd);