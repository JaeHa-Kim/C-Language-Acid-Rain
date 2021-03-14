#ifndef MY_UTIL_H
#define MY_UTIL_H

#include <time.h>
#include <Windows.h>

void GotoXY(int x, int y); // 커서 위치를 설정한다.
double GetDiffTime(clock_t start_time); // (현재-시작)시간을 초로 되돌려준다.
void SetCursorVisible(BOOL tf); // 커서를 안 보이게 만든다.
void SetConsoleSize(short x, short y); // 콘솔 크기를 결정한다.
#endif