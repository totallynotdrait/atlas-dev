#include "window.h"
#include <aagui/aagui.h>
#include <inc/mem/heap.h>

void moveWindow(Window WindowInfo, int newX, int newY) {
    WindowInfo.Position.X = newX;
    WindowInfo.Position.Y = newY;

    request_window(WindowInfo);
}

void request_window(Window WindowInfo) {
    log->print("Creating window...");
    if (WindowInfo.Maximized == true) {
        WindowInfo.Position.X = 0;
        WindowInfo.Position.Y = 0;
        WindowInfo.Dimensions.height = aaguir->TargetFramebuffer->Height;
        WindowInfo.Dimensions.width = aaguir->TargetFramebuffer->Width;
    }

    if (WindowInfo.Hidden == true) {
        log->warn("skipped...");
        return;
    }

    // main window
    drawRect(WindowInfo.Position.X, WindowInfo.Position.Y, WindowInfo.Dimensions.width, WindowInfo.Dimensions.height, WindowInfo.DefaultBackgroundColor, true);

    // "framebuffer"
    drawRect(WindowInfo.Position.X, WindowInfo.Position.Y+25, WindowInfo.Dimensions.width, WindowInfo.Dimensions.height-25, hbc.Black, true);

    // border
    drawRect(WindowInfo.Position.X, WindowInfo.Position.Y, WindowInfo.Dimensions.width, WindowInfo.Dimensions.height, WindowInfo.DefaultBorderColor, false);

    // 16x16 icon
    drawIcon_16(WindowInfo.SmallIconMask, WindowInfo.SmallIconMap, WindowInfo.Position.X + 5, WindowInfo.Position.Y + 5);

    // title
    aaguir->CursorPosition.X = WindowInfo.Position.X + 21;
    aaguir->CursorPosition.Y = WindowInfo.Position.Y + 5;
    aaguir->Color = WindowInfo.DefaultTitleColor;
    aaguir->printf(WindowInfo.Title);

    log->ok("Created window.");
}