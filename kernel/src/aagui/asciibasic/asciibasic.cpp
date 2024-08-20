#include "asciibasic.h"
#include <liba/stdio.h>
#include <kata_renderer/kata_renderer.h>

void makeRectangle(int x, int y, int width, int height) {
    int oldX = GKRenderer->CursorPosition.X;
    int oldY = GKRenderer->CursorPosition.Y;

    GKRenderer->CursorPosition.X = x;
    GKRenderer->CursorPosition.Y = y;
    for (int i = 0; i < width; ++i) {
        if (i == 0) {
            printf("+");
        } else if (i == width - 1) {
            printf("+\n");
        } else {
            printf("-");
        }
    }

    for (int i = 1; i < height - 1; ++i) {
        for (int j = 0; j < width; ++j) {
            if (j == 0 || j == width - 1) {
                printf("|");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }

    for (int i = 0; i < width; ++i) {
        if (i == 0) {
            printf("+");
        } else if (i == width - 1) {
            printf("+\n");
        } else {
            printf("-");
        }
    }

    GKRenderer->CursorPosition.X = oldX;
    GKRenderer->CursorPosition.Y = oldY;
}
