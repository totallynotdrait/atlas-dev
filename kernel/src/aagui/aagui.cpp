#include "aagui.h"

KAtaRenderer* aaguir;

void drawIcon_16(const uint8_t* iconMask, const uint32_t* iconMap, int X, int Y) {
    log->print("make icon");
    int iconSize = 16;
    int xMax = iconSize;
    int yMax = iconSize;

    int differenceX = aaguir->TargetFramebuffer->Width - X;
    int differenceY = aaguir->TargetFramebuffer->Height - Y;

    if (differenceX < iconSize) xMax = differenceX;
    if (differenceY < iconSize) yMax = differenceY;


    log->print("start first loop");
    for (int y = 0; y < yMax; y++) {
        log->print("start second loop");
        for (int x = 0; x < xMax; x++) {
            int bit = y * iconSize + x;
            int byte = bit / 8;
            if ((iconMask[byte] & (0b10000000 >> (x % 8)))) {
                log->print("true");
                uint32_t color = iconMap[y * iconSize + x];
                aaguir->putPix(X + x, Y + y, color);
            }
            log->print("false");

        }
    }
}

void drawText(int sX, int sY, uint32_t color, int size, const char* text) {
    /* char* chr = (char*) text;
    int i = 0;
    while(*chr != 0) 
    {
        aaguir->putChar(sX, sY, text[i]);
        sX += size * 8;
        i++;
    } */
}

void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, uint32_t col, int size) {
    drawLine(x1, y1, x2, y2, col, size);
    drawLine(x2, y2, x3, y3, col, size);
    drawLine(x3, y3, x1, y1, col, size);
}

void drawBlob(int x, int y, uint32_t col, int size) {
    for (int x1 = x - size; x1 <= x + size; x1++)
        for (int y1 = y - size; y1 <= y + size; y1++)
            aaguir->putPix(x1, y1, col);
}

void drawRect(int x, int y, int width, int height, uint32_t color, bool fill) {
    if (fill) {
        for (int x1 = x; x1 < x + width; x1++)
            for (int y1 = y; y1 < y + height; y1++)
                aaguir->putPix(x1, y1, color);
    }
    
    for (int x1 = x; x1 < x + width; x1++) {
        aaguir->putPix(x1, y, color);
        aaguir->putPix(x1, y + height - 1, color);
    }
    
    for (int y1 = y; y1 < y + height; y1++) {
        aaguir->putPix(x, y1, color);
        aaguir->putPix(x + width - 1, y1, color);
    }
}


void drawLine(int x0, int y0, int x1, int y1, uint32_t color, int size) {
    if (x0 == x1 || y0 == y1)
        {
            // fill with rect directly to make fast
            if (x0 > x1)
            {
                int temp = x0;
                x0 = x1;
                x1 = temp;
            }

            if (y0 > y1)
            {
                int temp = y0;
                y0 = y1;
                y1 = temp;
            }

            drawRect(
                x0 - size, 
                y0 - size, 
                x1 - x0 + 1 + size * 2, 
                y1 - y0 + 1 + size * 2, 
                color, true);
        }
        else
        {
            int dx = x1 - x0;
            int dy = y1 - y0;
            int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
            // float xInc = dx / (float)steps;
            // float yInc = dy / (float)steps;

            for (int i = 0; i < steps; i++)
            {
                drawBlob(
                    (x0 * steps + (dx * i)) / steps, 
                    (y0 * steps + (dy * i)) / steps,
                    color, 
                    size);
            }
        }
}