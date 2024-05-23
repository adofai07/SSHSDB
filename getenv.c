#include <windows.h>
#include "getenv.h"

int get_terminal_width() {
    CONSOLE_SCREEN_BUFFER_INFO c;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);

    return c.srWindow.Right - c.srWindow.Left + 1;
}

int get_terminal_height() {
    CONSOLE_SCREEN_BUFFER_INFO c;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &c);

    return c.srWindow.Bottom - c.srWindow.Top + 1;
}
