// Atlas Advanced Graphical User Interface
#pragma once
#include <stdint.h>
#include <stddef.h>
#include <kata_renderer/Framebuffer.h>
#include <kata_renderer/kata_renderer.h>

class Window;

struct WindowDimension {
    uint32_t width;
    uint32_t height;
};

class Window {
    public:
    const char* Title = "Incantevole";
    WindowDimension Dimensions = {450, 450};
    Point Position = {150, 150};
    uint32_t** OriginalPixels;

    const uint8_t SmallIconMask[32] = {
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
        0b11111111, 0b11111111,
    };

    const uint32_t SmallIconMap[256] = {
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
        dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,dbc.Blue,
    };
    
    bool ShowTitleBar;
    bool ShowBorder;
    bool Hidden;
    bool Moveable;
    bool Resizeable;
    bool Closeable;
    bool CaptureMouse;
    bool Maximized;

    bool IsActive;
    bool IsFrozen;

    uint32_t DefaultBorderColor = hbc.Gray;
    uint32_t SelectedBorderColor = hbc.Lime;
    uint32_t DefaultTitleColor = hbc.Black;
    uint32_t SelectedTitleColor = hbc.Gray;
    uint32_t DefaultTitleBackgroundColor = hbc.White;
    uint32_t DefaultBackgroundColor = hbc.White;

    uint32_t CurrentBorderColor;
    uint32_t CurrentTitleColor;
    uint32_t CurrentTitleBackgroundColor;

    uint32_t OldBorderColor;
    uint32_t OldTitleColor;
    uint32_t OldTitleBackgroundColor;

    FrameBuffer* Buffer;

    uint64_t ID;
    uint64_t PID;

};

void request_window(Window WindowInfo);
void moveWindow(Window WindowInfo, int newX, int newY);