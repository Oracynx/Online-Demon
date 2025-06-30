#include <iostream>
#include <windows.h>

#ifndef CONSOLE_LIBS

#define CONSOLE_LIBS

namespace Console
{
// 前景色常量
const int BLACK = 0;
const int DARK_BLUE = 1;
const int DARK_GREEN = 2;
const int DARK_CYAN = 3;
const int DARK_RED = 4;
const int DARK_MAGENTA = 5;
const int DARK_YELLOW = 6;
const int GRAY = 7;
const int DARK_GRAY = 8;
const int BLUE = 9;
const int GREEN = 10;
const int CYAN = 11;
const int RED = 12;
const int MAGENTA = 13;
const int YELLOW = 14;
const int WHITE = 15;

// 背景色常量
const int BG_BLACK = 0;
const int BG_DARK_BLUE = 16;
const int BG_DARK_GREEN = 32;
const int BG_DARK_CYAN = 48;
const int BG_DARK_RED = 64;
const int BG_DARK_MAGENTA = 80;
const int BG_DARK_YELLOW = 96;
const int BG_GRAY = 112;
const int BG_DARK_GRAY = 128;
const int BG_BLUE = 144;
const int BG_GREEN = 160;
const int BG_CYAN = 176;
const int BG_RED = 192;
const int BG_MAGENTA = 208;
const int BG_YELLOW = 224;
const int BG_WHITE = 240;

// 设置颜色
inline void SetColor(int foreground, int background = BG_BLACK)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int color = foreground | background;
    SetConsoleTextAttribute(hConsole, static_cast<WORD>(color));
}

// 重置颜色到默认（灰色前景，黑色背景）
inline void ResetColor()
{
    SetColor(GRAY, BG_BLACK);
}

// 设置光标位置
inline void SetCursorPosition(int x, int y)
{
    COORD coord = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 清屏
inline void ClearScreen()
{
    system("cls");
}
} // namespace Console

#endif