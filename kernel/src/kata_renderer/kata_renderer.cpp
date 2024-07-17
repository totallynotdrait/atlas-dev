#include "kata_renderer.h"
#include <mem/memory.h>
#include <aagui/aagui.h>
#include <scheduling/PIT/PIT.h>


HEXBasicColors hbc;
uint64_t cursorTimer = 0;
bool isMouseCursorVisible = true;
bool isCursorVisible = true;

KAtaRenderer* GKRenderer;

KAtaRenderer::KAtaRenderer(FrameBuffer* targetFramebuffer, PSF1_FONT* psf1_Font) {
    TargetFramebuffer = targetFramebuffer;
    PSF1_Font = psf1_Font;
    Color = hbc.Gray;
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
    CursorPosition.X = 0;
    CursorPosition.Y = 0;
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

void KAtaRenderer::drawCursor(bool on) {
    uint64_t font_height = 8;
    uint32_t color = on ? KAtaRenderer::Color : KAtaRenderer::ClearColor;
    drawRect(CursorPosition.X * 16, CursorPosition.Y * 8 + (8 / 4 * 3),
            16, 8 / 4, color, true);
    
}

void KAtaRenderer::scroll_up(FrameBuffer fb) {
    uint32_t width = fb.Width;
    uint32_t height = fb.Height;
    uint32_t char_height = 16;  // Assuming character height of 16 pixels

    // Adjust bytes_per_pixel to match the actual framebuffer color depth
    uint32_t bytes_per_pixel = 4;  // Assuming 32-bit color (4 bytes per pixel)

    uint32_t row_size = width * bytes_per_pixel;  // Size of one row in bytes

    // Calculate the size to move the screen content up by one character height
    uint32_t scroll_height = height - char_height;
    uint64_t copy_size = row_size * scroll_height;

    void* addr = fb.BaseAddress;

    // Move everything up by char_height rows
    memcpy(addr, (void*)((uint64_t)addr + row_size * char_height), copy_size);

    // Clear the last char_height rows
    uint64_t offset = row_size * scroll_height;
    void* last_row = (void*)((uint64_t)addr + offset);
    memset(last_row, 0, row_size * char_height);  // Clear with black color (0)
}



void KAtaRenderer::scanf(const char* str) {
    printf(str);
    isKeyboardEnabled = true;
}

void KAtaRenderer::putChar(char chr, unsigned int xOff, unsigned int yOff) {
    
    if (CursorPosition.X + 8 > TargetFramebuffer->Width) {
        CursorPosition.X = 0;
        CursorPosition.Y += 16;
    }

    if (CursorPosition.Y + 16 > TargetFramebuffer->Height){
        scroll_up(*TargetFramebuffer);
        CursorPosition.Y -= 16;
    }
    
    unsigned int *pixPtr = (unsigned int *)TargetFramebuffer->BaseAddress;
    char *fontPtr = ((char *)PSF1_Font->glyphBuffer) + (chr * PSF1_Font->psf1_Header->charsize);

    for (int64_t y = yOff; y < yOff + 16; y++)
    {
        for (int64_t x = xOff; x < xOff + 8; x++)
        {
            if (x >= 0 && x < TargetFramebuffer->Width && y >= 0 && y < TargetFramebuffer->Height)
                if ((*fontPtr & (0b10000000 >> (x - xOff))) > 0)
                {
                    *(unsigned int *)(pixPtr + x + (y * TargetFramebuffer->PixelsPerScanline)) = Color;
                }
        }
        fontPtr++;
    }

    
}

void KAtaRenderer::putChar(char chr) {
    
    putChar(chr, CursorPosition.X, CursorPosition.Y);
    CursorPosition.X += 8;
    
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

void KAtaRenderer::DrawOverlayMouseCursor(uint8_t* mouseCursor, Point position, uint32_t* colorMap) {
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
                putPix(position.X + x, position.Y + y, colorMap[y * 16 + x]);
                MouseCursorBufferAfter[x + y * 16] = getPix(position.X + x, position.Y + y);
            }
        }
    }
    
    MouseDrawn = true;
}

void KAtaRenderer::paint() {
    PIT::Sleep(10000);
    isCursorVisible = !isCursorVisible;

    if (isCursorVisible) {
        KAtaRenderer::drawCursor(true);
    } else {
        KAtaRenderer::drawCursor(false);
    }
}