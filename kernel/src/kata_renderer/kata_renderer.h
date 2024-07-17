#pragma once
#include "Framebuffer.h"
#include "font/Fonts.h"
#include <math/math.h>
#include <stdint.h>
#include <userinput/keyboard.h>
#include <userinput/kbScancode.h>
#include <interrupts/IDT.h>
#include <interrupts/interrupts.h>
#include <interrupts/interrupts.h>
#include <intnum/cstr.h>
#include <inc/userinput/mouse.h>
#include <aagui/rendering/BasicColors.h>

extern bool isMouseCursorVisible;

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
    void scroll_up(FrameBuffer fb);
    void Next();
    void DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t* colorMap);
    void ClearMouseCursor(uint8_t* mouseCursor, Point position);
    void drawCursor(bool on);
    void paint();
    bool MouseDrawn;
    
};

extern KAtaRenderer* GKRenderer;