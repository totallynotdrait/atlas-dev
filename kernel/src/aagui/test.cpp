#include "test.h"

Window* hwnd;

int main() {
    GKRenderer->printf(to_hexstring((uint64_t)hwnd));
    return 0;
}