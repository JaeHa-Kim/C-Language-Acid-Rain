#include <Windows.h>
#include <stdio.h> // SetConsoleSize
#include "util.h"

void GotoXY(int x, int y)
{
    COORD Pos = { x, y };
    Pos.X = x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

double GetDiffTime(clock_t start_time)
{
    clock_t cur_time;
    cur_time = clock();
    return ((double)(cur_time - (double)start_time) / CLOCKS_PER_SEC);
}

void SetCursorVisible(BOOL tf)
{
    CONSOLE_CURSOR_INFO ci = { 10, tf };
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ci);
}

void SetConsoleSize(short x, short y)
{
    COORD coord = { x,y };
    SMALL_RECT windowSize = { 0, 0, coord.X-1, coord.Y-1 }; // LEFT, TOP, RIGHT, BOOTOM
    HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE); // 핸들 얻기
    SetConsoleScreenBufferSize(wHnd, coord); // 버퍼 사이즈 설정
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize); // 윈도우 사이즈 설정
}