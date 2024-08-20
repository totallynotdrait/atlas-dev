#include "rtc.h"

#include <IO/IO.h>

tm *timedate;

uint8_t GetYearRTC()
{
	outb(0x70, 0x09);
	return inb(0x71);
}

uint8_t GetMonthRTC()
{
	outb(0x70, 0x08);
	return inb(0x71);
}

uint8_t GetDayRTC()
{
	outb(0x70, 0x07);
	return inb(0x71);
}

uint8_t GetWeekdayRTC()
{
	outb(0x70, 0x06);
	return inb(0x71);
}

uint8_t GetHourRTC()
{
	outb(0x70, 0x04);
	return inb(0x71);
}

uint8_t GetMinuteRTC()
{
	outb(0x70, 0x02);
	return inb(0x71);
}

uint8_t GetSecondRTC()
{
	outb(0x70, 0x00);
	return inb(0x71);
}

void InitRTC(struct tm* kernel_time)
{
	log->print("Initializing Real Time Clock (RTC)...");
	GetTimeRTC(kernel_time);
	log->print("Initialized Real Time Clock (RTC).");
}

void GetTimeRTC(struct tm* tm)
{
	tm->year = GetYearRTC();
	tm->month = GetMonthRTC();
	tm->day = GetDayRTC();
	tm->hour = GetHourRTC();
	tm->minute = GetMinuteRTC();
	tm->second = GetSecondRTC();
}

uint8_t BCDToBinary(uint8_t bcd)
{
    return ((bcd / 16) * 10) + (bcd % 16);
}