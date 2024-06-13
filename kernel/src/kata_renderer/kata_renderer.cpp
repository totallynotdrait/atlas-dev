#include "kata_renderer.h"


KAtaRenderer* GKRenderer;

KAtaRenderer::KAtaRenderer(FrameBuffer* targetFramebuffer, PSF1_FONT* psf1_Font) {
    TargetFramebuffer = targetFramebuffer;
    PSF1_Font = psf1_Font;
    Color = 0xffffffff;
    CursorPosition = {0, 0};
}

void KAtaRenderer::Clear(){
    uint64_t fbBase = (uint64_t)TargetFramebuffer->BaseAddress;
    uint64_t bytesPerScanline = TargetFramebuffer->PixelsPerScanline * 4;
    uint64_t fbHeight = TargetFramebuffer->Height;
    uint64_t fbSize = TargetFramebuffer->BufferSize;

    for (int verticalScanline = 0; verticalScanline < fbHeight; verticalScanline ++){
        uint64_t pixPtrBase = fbBase + (bytesPerScanline * verticalScanline);
        for (uint32_t* pixPtr = (uint32_t*)pixPtrBase; pixPtr < (uint32_t*)(pixPtrBase + bytesPerScanline); pixPtr ++){
            *pixPtr = ClearColor;
        }
    }
}

void KAtaRenderer::ClearChar() {
    if (CursorPosition.X == 0) {
        CursorPosition.X = TargetFramebuffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0) CursorPosition.Y = 0;
    }

    unsigned int xOff = CursorPosition.X;
    unsigned int yOff = CursorPosition.Y;

    unsigned int* pixPtr =  (unsigned int*)TargetFramebuffer->BaseAddress;
    for (unsigned long y = yOff; y < yOff + 16; y++) {
        for (unsigned long x = xOff - 8; x < xOff; x++) {
                *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanline)) = ClearColor;
        }
    }

    CursorPosition.X -= 8;
    if (CursorPosition.X < 0) {
        CursorPosition.X = TargetFramebuffer->Width;
        CursorPosition.Y -= 16;
        if (CursorPosition.Y < 0) CursorPosition.Y = 0;
    }
}

void KAtaRenderer::Next(){
    CursorPosition.X = 0;
    CursorPosition.Y += 16;

}

void KAtaRenderer::printf(const char* str) {
    char* chr = (char*) str;
    while(*chr != 0) {
        putChar(*chr, CursorPosition.X, CursorPosition.Y);
        CursorPosition.X+=8;
        if (CursorPosition.X + 8 > TargetFramebuffer->Width)
        {
            CursorPosition.X = 0;
            CursorPosition.Y += 16;
        }
        chr++;
    }
}

void KAtaRenderer::scanf(const char* str) {
    printf(str);
    isKeyboardEnabled = true;
}

void KAtaRenderer::putChar(char chr, unsigned int xOff, unsigned int yOff) {
    if (xOff + 8 > TargetFramebuffer->Width || yOff + 16 > TargetFramebuffer->Height) return;
    unsigned int* pixPtr =  (unsigned int*)TargetFramebuffer->BaseAddress;
    char* fontPtr = (char*)PSF1_Font->glyphBuffer + (chr * PSF1_Font->psf1_Header->charsize);
    for (unsigned long y = yOff; y < yOff + 16; y++) {
        for (unsigned long x = xOff; x < xOff + 8; x++) {
            if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0) {
                *(unsigned int*)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanline)) = Color;
            }
        }
        fontPtr++;
    }
}

void KAtaRenderer::putChar(char chr) {
    putChar(chr, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X += 8;
    if (CursorPosition.X + 8 > TargetFramebuffer->Width) {
        CursorPosition.X = 0;
        CursorPosition.Y += 16;
    }
}

void KAtaRenderer::putPix(uint32_t x, uint32_t y, uint32_t color) {
    *(uint32_t*)((uint64_t)TargetFramebuffer->BaseAddress + (x*4) + (y * TargetFramebuffer->PixelsPerScanline * 4)) = color;
}

uint32_t KAtaRenderer::getPix(uint32_t x, uint32_t y) {
    return *(uint32_t*)((uint64_t)TargetFramebuffer->BaseAddress + (x*4) + (y * TargetFramebuffer->PixelsPerScanline * 4));
}

void KAtaRenderer::ClearMouseCursor(uint8_t* mouseCursor, Point position) {
    if (!MouseDrawn) return;
    int xMax = 16;
    int yMax = 16;
    int differenceX = TargetFramebuffer->Width - position.X;
    int differenceY = TargetFramebuffer->Height - position.Y;

    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int y = 0; y < yMax; y++) {
        for (int x = 0; x < xMax; x++) {
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouseCursor[byte] & (0b10000000 >> (x % 8)))) {
                if (getPix(position.X + x, position.Y + y) == MouseCursorBufferAfter[x + y * 16]) {
                    putPix(position.X + x, position.Y + y, MouseCursorBuffer[x + y * 16]);
                }
                
            }
        }
        
    }
}

void KAtaRenderer::DrawOverlayMouseCurosr(uint8_t* mouseCursor, Point position, uint32_t color) {
    
    
    int xMax = 16;
    int yMax = 16;
    int differenceX = TargetFramebuffer->Width - position.X;
    int differenceY = TargetFramebuffer->Height - position.Y;

    if (differenceX < 16) xMax = differenceX;
    if (differenceY < 16) yMax = differenceY;

    for (int y = 0; y < yMax; y++){
        for (int x = 0; x < xMax; x++){
            int bit = y * 16 + x;
            int byte = bit / 8;
            if ((mouseCursor[byte] & (0b10000000 >> (x % 8)))){
                MouseCursorBuffer[x + y * 16] = getPix(position.X + x, position.Y + y);
                putPix(position.X + x, position.Y + y, color);
                MouseCursorBufferAfter[x + y * 16] = getPix(position.X + x, position.Y + y);
            }
        }
    }
    
    MouseDrawn = true;
}