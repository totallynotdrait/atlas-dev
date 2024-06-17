// Atlas Advanced Graphical User Interface
#pragma once
#include <stdint.h>
#include <stddef.h>
#include "../../kata_renderer/FrameBuffer.h"

class Window;

struct WindowDimension {
    int width;
    int height;
};

class Window {
    public:
    const char* Title;
    WindowDimension Dimesions = {450,450};

    bool ShowTitleBar;
    bool ShowBorder;
    bool Hidden;
    bool Moveable;
    bool Resizeable;
    bool Closeable;
    bool CaptureMouse;

    bool IsActive;
    bool IsFrozen;

    uint32_t DefaultBorderColor;
    uint32_t SelectedBorderColor;
    uint32_t DefaultTitleColor;
    uint32_t SelectedTitleColor;
    uint32_t DefaultTitleBackgroundColor;
    uint32_t DefaultBackgroundColor;

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