#pragma once
#include "rendering/BasicColors.h"
#include "window/window.h"

extern KAtaRenderer* aaguir;

void drawIcon_16(const uint8_t* iconMask, const uint32_t* iconMap, int X, int Y);
void drawText(int sX, int sY, uint32_t color, int size, const char* text);
void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t col, int size);
void drawBlob(int x, int y, uint32_t col, int size);
void drawRect(int x, int y, int width, int height, uint32_t color, bool fill);
void drawLine(int x0, int y0, int x1, int y1, uint32_t color, int size);