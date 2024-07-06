#pragma once
#include <IO/IO.h>
#include <math/math.h>
#include <kata_renderer.h>

#define PS2Leftbutton 0b00000001
#define PS2Middlebutton 0b00000100
#define PS2Rightbutton 0b00000010
#define PS2XSign 0b00010000
#define PS2YSign 0b00100000
#define PS2XOverflow 0b01000000
#define PS2YOverflow 0b10000000

extern uint8_t MousePointerBuffer[];
extern uint32_t MousePointerMap[];

void InitPS2Mouse();
void HandlePS2Mouse(uint8_t data);
void ProcessMousePacket();

void showMouseCursor();
void hideMouseCursor();
extern Point MousePosition;
extern bool LeftClicked;
extern bool MiddleClicked;
extern bool RightClicked;