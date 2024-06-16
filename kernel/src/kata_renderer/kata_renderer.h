#pragma once
#include "../math/math.h"
#include "Framebuffer.h"
#include "font/Fonts.h"
#include <stdint.h>
#include "../base_utils/userinput/keyboard.h"
#include "../base_utils/userinput/kbScancode.h"
#include "../base_utils/interrupts/IDT.h"
#include "../base_utils/interrupts/interrupts.h"
#include "../base_utils/interrupts/interrupts.h"
#include "../base_utils/intnum/cstr.h"


class KAtaRenderer {
    public:
    KAtaRenderer(FrameBuffer* targetFramebuffer, PSF1_FONT* psf1_Font);
    Point CursorPosition;
    FrameBuffer* TargetFramebuffer;
    PSF1_FONT* PSF1_Font;
    uint32_t MouseCursorBuffer[16 * 16];
    uint32_t MouseCursorBufferAfter[16 * 16];
    unsigned int Color;
    unsigned int ClearColor;
    void printf(const char* str);
    void scanf(const char* str);
    void putChar(char chr, unsigned int xOff, unsigned int yOff);
    void putChar(char chr);
    void putPix(uint32_t x, uint32_t y, uint32_t color);
    uint32_t getPix(uint32_t x, uint32_t y);
    void ClearChar();
    void Clear();
    void Next();
    void Scroll();
    void DrawOverlayMouseCurosr(uint8_t* mouseCursor, Point position, uint32_t color);
    void ClearMouseCursor(uint8_t* mouseCursor, Point position);
    bool MouseDrawn;
};

extern KAtaRenderer* GKRenderer;