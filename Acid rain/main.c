/* include */
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h> // clock_t
#include "util.h"
#include <mmsystem.h> // wav 재생
#pragma comment(lib,"winmm.lib") // 시간 측정

/* 단어 설정 */
#define MAX_WORD_LENGTH 17 // 입력 가능한 최대 단어 길이
#define MAX_SCREEN_WORD_COUNT 40 // 스테이지에서 불러올 수 있는 최대 단어 개수. 매 스테이지마다 다른 단어가 나옴.
#define WORD_COUNT 1000 // 단어장에서 불러올 단어의 개수

/* 기본 게임 설정 */
int passing_score = 5; // 몇 점을 모아야 클리어인가
int Number_of_stages = 5; // 몇 스테이지까지 가야 VICTORY인가
int g_life = 5; // 목숨
double g_falling_speed = 1.5; // 단어 낙하 주기(초). 새로운 단어가 내려오면서 전체적으로 단어들이 낙하한다. 새로운 단어가 생성되는 주기와 같다.

/* 좌표 */
int input_x = 51; // 입력창 좌표
int input_y = 28; // 입력창 좌표
int score_x = 0; // 점수창 좌표
int life_x = 12; // 목숨창 좌표
int stage_x = 101; // STAGE창 좌표

/* 변수, 정의 초기화 */
#define KEY_ENTER '\r' // 엔터키
#define KEY_BS '\b' // 백스페이스
typedef struct ScreenWord
{
	int index;
	int x;
	int y;
} ScreenWord;
ScreenWord g_screen_word[MAX_SCREEN_WORD_COUNT]; // 스테이지에 출현하는 단어들이 저장된 장소
char** g_words; // 단어장. 메모장에서 불러온 단어들이 저장된 곳
char g_input_word[MAX_WORD_LENGTH + 1]; // 현재 입력 중인 단어 배열
int g_input_word_length = 0; // 현재 입력 중인 단어의 길이 저장
int g_score = 0; // 현재 점수
int stage = 1; // 현재 스테이지 저장
clock_t g_start_time; // 시간 측정
int play_record = 0; // 이 전에 재생한 음악의 번호를 기록하는 변수
int whether_repeated = 0; // 음악을 반복재생하면 1이 되고 다른 음악을 찾는다.
int* infinite_score = &passing_score; // 무한모드용 목표 점수
int* infinite_stages = &Number_of_stages; // 무한모드용 목표 스테이지
int* infinite_life = &g_life; // 무한모드용 목숨

void StartGame(void);
void InitScreen(void);
void NextStage(void);
void StageScreen(void);
void UpdateScore(void);
void UpdateLifeCount(void);
void UpdateStage(void);
void Run(void);
void InitData(void);
void InitWord(int i);
void ProcessInput(int key);
void CompareWords(void);
void EraseWord(int i);
void DrawWord(int i);
void FreeWord(void);
void Random_music(void);
void Infinite_mode_setting(void);

int main(void)
{
	SetConsoleSize(120, 30); // 콘솔 크기를 결정한다.
	system("title ACID RAIN"); // 콘솔 제목을 설정한다.
	SetCursorVisible(FALSE); // 커서를 보이지 않게 만든다.
	clock_t g_start_time = clock(); // 기준 시간을 설정한다.
	srand((unsigned int)time(NULL)); // 난수 생성 준비물

	// 동작 함수
	InitData();
	StartGame();
	InitScreen();
	Run();

	return 0;
}

// 무한모드에 전용세팅입니다.
void Infinite_mode_setting(void)
{
	*infinite_score = 15;
	*infinite_stages = 99;
	*infinite_life = 99;
}

// 메인 화면을 출력한다.
void StartGame(void)
{
	int main_y = 5; // 좌표값을 쉽게 움직이기 위해서 사용한다.
	int key_input;

	system("cls");
	PlaySound(TEXT("Main screen background sound.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);	

	// 문자(■)는 최대 (60, 15)개가 들어간다.
	// 좌표는 최대 (120, 30)
	// x좌표 22.5
	GotoXY(23, main_y);     printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
	GotoXY(23, main_y + 1); printf("■■□□■■■□□■■□■□□□■■■■□□□■■■□□■■□■□■■□■");
	GotoXY(23, main_y + 2); printf("■□■■□■□■■□■□■□■■□■■■□■■□■□■■□■□■□□■□■");
	GotoXY(23, main_y + 3); printf("■□□□□■□■■■■□■□■■□■■■□□□■■□□□□■□■□□□□■");
	GotoXY(23, main_y + 4); printf("■□■■□■□■■□■□■□■■□■■■□■■□■□■■□■□■□■□□■");
	GotoXY(23, main_y + 5); printf("■□■■□■■□□■■□■□□□■■■■□■■□■□■■□■□■□■■□■");
	GotoXY(23, main_y + 6); printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

	while (1)
	{
		if (_kbhit())
		{
			key_input = _getch();
			if (key_input == '`')
			{
				Infinite_mode_setting();
				PlaySound(TEXT("Infinite mode start sound.wav"), NULL, SND_SYNC | SND_NODEFAULT);
				break;
			}
			else
			{
				PlaySound(TEXT("Press the start button.wav"), NULL, SND_SYNC | SND_NODEFAULT);
				break;
			}
		}

		GotoXY(49, main_y + 13); printf("Press any key to start.");
		Sleep(1200);

		if (_kbhit())
		{
			key_input = _getch();
			if (key_input == '`')
			{
				Infinite_mode_setting();
				PlaySound(TEXT("Infinite mode start sound.wav"), NULL, SND_SYNC | SND_NODEFAULT);
				break;
			}
			else
			{
				PlaySound(TEXT("Press the start button.wav"), NULL, SND_SYNC | SND_NODEFAULT);
				break;
			}
		}

		GotoXY(49, main_y + 13); printf("                       ");
		Sleep(1200);
	}
}

// 메모장에서 단어를 불러와서 각각의 단어를 배열에 저장한다.
void InitData(void)
{
	int i;
	FILE* fp = NULL;

	if (0 == fopen_s(&fp, "g_words.txt", "r"));

	// g_words: 단어장 전체 공간
	g_words = (char**)malloc(sizeof(char*) * (WORD_COUNT + 1));

	if (g_words != NULL) // 보안 경고 해제
	{
		for (i = 0; i < WORD_COUNT; i++)
		{
			if (g_words[i] != NULL) // 한 단어마다 [i]에 넣어서 배열으로 만든다.
			{
				g_words[i] = (char*)malloc(sizeof(char) * (MAX_WORD_LENGTH + 1));
				fgets(g_words[i], sizeof(char) * MAX_WORD_LENGTH, fp);
				g_words[i][strlen(g_words[i]) - 1] = '\0'; // fgets를 사용하여 반드시 마지막에 \n\0가 있어서 \n을 \0로 바꿔준다.
			}
		}
	}

	g_words[WORD_COUNT] = " ";

	fclose(fp);
}

// 게임을 플레이하는 스테이지 화면을 그린다.
void InitScreen(void)
{
	system("cls");
	Random_music();

	int i;
	int line_x = 115; // 테두리 줄을 그릴 때 사용한다.

	// 최대 좌표 (120-2, 30-1)

	// 모서리
	GotoXY(0, 0); printf("┌");
	GotoXY(118, 0); printf("┐");
	GotoXY(0, 26); printf("└");
	GotoXY(118, 26); printf("┘");

	for (i = 0; i < line_x; i++) // 상단줄
	{
		GotoXY(i + 2, 0);
		printf("─");
	}
	
	for (i = 0; i < 25; i++) // 사이드줄
	{
		GotoXY(0, i + 1);
		printf("│");

		GotoXY(118, i + 1);
		printf("│");
	}

	for (i = 0; i < line_x; i++) // 하단줄
	{
		GotoXY(i + 2, 26); // 더 아래로 내리면 줄이 겹친다.
		printf("─");
	}
	
	// 좌표 48.5
	GotoXY(input_x - 2, input_y - 1); printf("┌──────────────────┐");
	GotoXY(input_x - 2, input_y);     printf("│                  │");
	GotoXY(input_x - 2, input_y + 1); printf("└──────────────────┘");

	// 점수칸
	GotoXY(score_x, 27); printf("┌───────────┐");
	GotoXY(score_x, 28); printf("│ SCORE:    │");
	GotoXY(score_x, 29); printf("└───────────┘");
	UpdateScore();

	// 목숨칸
	GotoXY(life_x, 27); printf("┌───────────┐");
	GotoXY(life_x, 28); printf("│ LIFE:     │");
	GotoXY(life_x, 29); printf("└───────────┘");
	UpdateLifeCount(); // 상자와 변수를 분리해야 문자 밀림 현상이 없다.

	// 스테이지 정보
	GotoXY(stage_x, 27); printf("┌───────────────┐");
	GotoXY(stage_x, 28); printf("│ STAGE:    /   │");
	GotoXY(stage_x, 29); printf("└───────────────┘");
	UpdateStage();
}

// 스테이지 정보를 화면에 출력한다.
void UpdateStage(void)
{
	GotoXY(stage_x + 9, 28); printf("%d", stage);
	GotoXY(stage_x + 14, 28); printf("%d", Number_of_stages);
}

// 점수를 화면에 출력한다.
void UpdateScore(void)
{
	GotoXY(score_x + 9, 28); printf("%d", g_score);
}

// 목숨을 화면에 출력한다.
void UpdateLifeCount(void)
{
	GotoXY(life_x + 8, 28); printf("%d", g_life);
}

// 게임을 동작하게 만드는 함수가 모여있다.
void Run(void)
{
	int i, count, key;
	count = 1;

	// 최대 출력 단어의 개수만큼 단어의 위치를 랜덤값으로 설정한다.
	for (i = 0; i < MAX_SCREEN_WORD_COUNT; i++)
	{
		InitWord(i);
	}

	while (1)
	{
		if (_kbhit()) // 키를 입력 받으면 ProcessInput 함수에 넣어라
		{
			key = _getch();
			ProcessInput(key);
		}
		if (GetDiffTime(g_start_time) > g_falling_speed) // g_falling_speed 시간을 초과할 경우
		{
			for (i = 0; i < count; i++)
			{
				EraseWord(i);
				g_screen_word[i].y++;
				if (g_screen_word[i].y == 26) // [i] 단어가 마지막 줄에 도착했을 경우
				{
					if (g_screen_word[i].index != WORD_COUNT) // [i]의 인덱스값이 초기화되지 않았을 경우. 단어를 맞추지 못했다는 말.
					{
						// 단어를 못 맞췄다면 목숨이 깍이고, 목숨 업데이트 후, 게임 오버 조건에 해당되는지 확인한다.
						g_life--;
						UpdateLifeCount();
						NextStage();
					}
					InitWord(i);
					g_screen_word[i].y++;
					DrawWord(i);
				}
				else
					DrawWord(i); // 마지막 줄이 아니면 단어를 지우고 y 좌표를 이동시키고 그려라.

			}
			if (count < MAX_SCREEN_WORD_COUNT)
				count++;

			g_start_time = clock(); // 기준 시간 초기화
		}
	}
}

// 단어 위치 설정
void InitWord(int i)
{
	int max_x = 115; // 선을 침범하지 않는 선에서의 최대 x 좌표값. 출력 범위 0~115

	g_screen_word[i].index = rand() % WORD_COUNT; // 인덱스 번호를 부여한다.
	g_screen_word[i].x = (rand() % max_x) + 2; // x 좌표 범위. +2 를 하는 이유는 선을 침범하지 않으려고. 2~117
	g_screen_word[i].y = 0; // y 좌표 범위. 0으로 설정하면 1부터 출발한다.
		
	// 단어 길이 + 출력 범위가 선을 넘을 경우
	if (g_screen_word[i].x + (int)strlen(g_words[g_screen_word[i].index]) > (max_x + 2))
	{
		// 선을 넘지 않는 좌표값이 나올 때까지 반복한다.
		while (1)
		{
			g_screen_word[i].x = (rand() % max_x) + 2;
			if (g_screen_word[i].x + (int)strlen(g_words[g_screen_word[i].index]) < (max_x + 2))
				break;
		}
	}
}

// 키워드 입력창 기능 함수
void ProcessInput(int key)
{
	if (key == KEY_BS) // 입력창에서 백스페이스를 눌렀을 때
	{
		if (g_input_word_length > 0) // 입력 중인 단어가 1글자라도 있을 때
		{
			GotoXY(input_x + g_input_word_length - 1, input_y); // 입력 중인 키워드의 마지막 좌표로 이동한다.
			g_input_word[g_input_word_length] = '\0'; // 입력 중인 단어 배열의 마지막을 비운다.
			printf(" "); // 문자가 있던 곳에 공백을 출력한다.
			g_input_word_length -= 2;
		}
	}
	if (key != KEY_ENTER) // 엔터키가 아니라는 것은 알파벳을 말한다.
	{
		if (g_input_word_length != MAX_WORD_LENGTH) // 입력 중인 단어가 최대 길이가 아닐 경우
		{
			GotoXY(input_x + g_input_word_length, input_y); // 마지막 입력 위치로 이동한다.
			printf("%c", key); // 키보드로 입력되는 키를 화면에 출력한다.
			g_input_word[g_input_word_length] = key; // 입력 중인 단어를 저장하는 배열에도 키보드 입력을 추가한다.
			g_input_word_length++; // 단어의 길이 변수에도 1을 추가한다.
		}
	}
	if (key == KEY_ENTER)
	{
		int count;
		CompareWords();
		GotoXY(input_x, input_y);
		for (count = 0; count < (int)strlen(g_input_word); count++)
		{
			printf(" "); // 입력 중인 단어의 길이를 받아서 길이만큼 공백처리하여 제거한다.
		}
		for (count = 0; count < g_input_word_length; count++)
		{
			g_input_word[count] = '\0'; // g_input_word[i]의 내용을 전부 초기화한다.
		}
		g_input_word_length = 0; // 그 후 단어 길이도 초기화한다.
	}
	NextStage();
}

// 출력된 문자와 같은 문자가 있으면 화면에서 제거한다.
void CompareWords(void)
{
	int i;
	int	Delete = -1;

	for (i = 0; i < MAX_SCREEN_WORD_COUNT; i++) // 출력 가능한 단어만큼 반복한다.
	{
		// 화면에 출력된 단어와 입력한 단어가 똑같을 경우
		if (strcmp(g_input_word, g_words[g_screen_word[i].index]) == 0)
		{
			if (Delete != -1) // 두번째부터 여기서 시작한다. 두번째부터는 같은 단어가 있을 때 아래쪽에 있는 단어를 선택해서 삭제한다.
			{
				if (g_screen_word[i].y > g_screen_word[Delete].y) // 같은 단어가 있을 때, 아래쪽에 있는 단어를 선택한다.
					Delete = i;
			}
			if (Delete == -1) // 처음 엔터키 누르면 여기로 가서 Delete는 i가 된다.
				Delete = i;
		}
	}

	if (Delete != -1) // 한 번이라도 정답을 입력했을 경우
	{
		EraseWord(Delete); // 정답을 입력한 [i]에 가서 제거한다.
		g_screen_word[Delete].index = WORD_COUNT; // 정답을 입력한 [i]의 인덱스값을 초기화한다.
		g_score++; // 점수를 +1 한다.
		UpdateScore(); // 점수를 업데이트한다.
	}
}

// g_screen_word[i]에 저장된 xy좌표로 가서 단어의 길이만큼 공백으로 출력한다.
void EraseWord(int i)
{
	int count;
	if (g_screen_word[i].y > 0) // y > 0 은 이미 출발한 단어를 가리킨다.
	{
		GotoXY(g_screen_word[i].x, g_screen_word[i].y);
		for (count = 0; count < (int)strlen(g_words[g_screen_word[i].index]); count++)
			printf(" ");
	}
}

// g_screen_word[i]에 저장된 xy좌표로 이동해서 단어를 출력한다.
void DrawWord(int i)
{
	GotoXY(g_screen_word[i].x, g_screen_word[i].y);
	// g_screen_word에는 문자열이 저장되어 있지 않기 때문에 index값을 이용해서 g_words로부터 불러와야 한다.
	printf("%s", g_words[g_screen_word[i].index]); 
}

// GAME OVER 화면과 다음 스테이지로 넘어가는 조건을 검사한다.
void NextStage(void)
{
	if (0 == g_life)
	{
		system("cls");

		// 게임 오버 좌표
		int over_x = 12; 
		int over_y = 8;

		GotoXY(over_x, over_y);     printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
		GotoXY(over_x, over_y + 1); printf("■■□□□■■■□□■■□■■■□■□□□□■■■■□□□■■□■■■□■□□□□■□□□■■");
		GotoXY(over_x, over_y + 2); printf("■□■■■■■□■■□■□□■□□■□■■■■■■□■■■□■□■■■□■□■■■■□■■□■");
		GotoXY(over_x, over_y + 3); printf("■□■□□□■□□□□■□■□■□■□□□□■■■□■■■□■■□■□■■□□□□■□□□■■");
		GotoXY(over_x, over_y + 4); printf("■□■■■□■□■■□■□■■■□■□■■■■■■□■■■□■■□■□■■□■■■■□■■□■");
		GotoXY(over_x, over_y + 5); printf("■■□□□■■□■■□■□■■■□■□□□□■■■■□□□■■■■□■■■□□□□■□■■□■");
		GotoXY(over_x, over_y + 6); printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

		PlaySound(TEXT("Game over screen.wav"), NULL, SND_ASYNC | SND_NODEFAULT);

		while (1)
		{
			GotoXY(44, 20); printf("Press any key to end the game.");
			Sleep(1000);

			if (_kbhit())
			{
				FreeWord();
				exit(0);
			}

			GotoXY(44, 20); printf("                              ");
			Sleep(1000);

			if (_kbhit())
			{
				FreeWord();
				exit(0);

			}
		}
	}

	// 기준 점수와 모은 점수가 같으면 난이도를 올리고, 점수를 초기화하고, 점수와 목숨을 업데이트하고, 다음 화면을 출력한다.
	if (g_score == passing_score) 
	{
		g_falling_speed *=  0.85; // 매 단계마다 속도가 복리로 x% 증가한다.
		g_score = 0;
		UpdateScore();
		UpdateLifeCount();
		StageScreen();
	}
}

// NEXT STAGE와 VICTORY 화면 출력
void StageScreen(void)
{
	system("cls");

	// NEXT STAGE 좌표
	int stage_x = 11;
	int stage_y = 9;

	int vic_x = 20; // 승리시 화면. 좌표 19~20

	if (stage < Number_of_stages) // 설정된 스테이지 레벨보다 낮을 때 출력한다.
	{
		GotoXY(stage_x, stage_y);     printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
		GotoXY(stage_x, stage_y + 1); printf("■□■■□■□□□□■□■■■□■□□□□□■■■□□□■□□□□□■■□□■■■□□■■□□□□■");
		GotoXY(stage_x, stage_y + 2); printf("■□□■□■□■■■■■□■□■■■■□■■■■□■■■■■■□■■■□■■□■□■■■■□■■■■");
		GotoXY(stage_x, stage_y + 3); printf("■□□□□■□□□□■■■□■■■■■□■■■■■□□■■■■□■■■□□□□■□■□□■□□□□■");
		GotoXY(stage_x, stage_y + 4); printf("■□■□□■□■■■■■□■□■■■■□■■■■■■■□■■■□■■■□■■□■□■■□■□■■■■");
		GotoXY(stage_x, stage_y + 5); printf("■□■■□■□□□□■□■■■□■■■□■■■■□□□■■■■□■■■□■■□■■□□■■□□□□■");
		GotoXY(stage_x, stage_y + 6); printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

		PlaySound(TEXT("Next stage screen.wav"), NULL, SND_ASYNC | SND_NODEFAULT);

		// 좌표 41
		GotoXY(41, stage_y + 10); printf("After 5 second, start the next stage.");
		Sleep(1000);
		GotoXY(41, stage_y + 10); printf("After 4 second, start the next stage.");
		Sleep(1000);
		GotoXY(41, stage_y + 10); printf("After 3 second, start the next stage.");
		Sleep(1000);
		GotoXY(41, stage_y + 10); printf("After 2 second, start the next stage.");
		Sleep(1000);
		GotoXY(41, stage_y + 10); printf("After 1 second, start the next stage.");
		Sleep(1000);
		GotoXY(41, stage_y + 10); printf("After 0 second, start the next stage.");

		stage += 1;
		InitScreen();
		Run();
	}
	else // 설정된 스테이지값과 같거나 크면 승리 화면을 출력한다.
	{
		GotoXY(vic_x, stage_y);     printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");
		GotoXY(vic_x, stage_y + 1); printf("■□■■■□■□■■□□□■■□□□□□■■□□□■■□□□■■□■■■□■□■");
		GotoXY(vic_x, stage_y + 2); printf("■□■■■□■□■□■■■□■■■□■■■□■■■□■□■■□■■□■□■■□■");
		GotoXY(vic_x, stage_y + 3); printf("■■□■□■■□■□■■■■■■■□■■■□■■■□■□□□■■■■□■■■□■");
		GotoXY(vic_x, stage_y + 4); printf("■■□■□■■□■□■■■□■■■□■■■□■■■□■□■■□■■■□■■■■■");
		GotoXY(vic_x, stage_y + 5); printf("■■■□■■■□■■□□□■■■■□■■■■□□□■■□■■□■■■□■■■□■");
		GotoXY(vic_x, stage_y + 6); printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■");

		PlaySound(TEXT("Victory Screen.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);

		while (1)
		{
			GotoXY(44, stage_y + 10); printf("You have completed all stages!"); // 좌표 44~45
			Sleep(2500);

			if (_kbhit())
			{
				FreeWord();
				exit(0);
			}

			GotoXY(44, stage_y + 10); printf("                              ");
			Sleep(1250);

			if (_kbhit())
			{
				FreeWord();
				exit(0);
			}

			GotoXY(48, stage_y + 10); printf("Press any key to exit."); // 48~49
			Sleep(2500);

			if (_kbhit())
			{
				FreeWord();
				exit(0);
			}

			GotoXY(48, stage_y + 10); printf("                      ");
			Sleep(1250);

			if (_kbhit())
			{
				FreeWord();
				exit(0);
			}
		}
	}
}

// 단어장 전체와 단어들에게 할당한 모든 동적 메모리 할당을 해제한다.
void FreeWord(void)
{
	int i;

	for (i = 0; i < WORD_COUNT; i++)
	{
		free(g_words[i]); // 개별 단어
	}

	free(g_words); // 단어장
}

// 무작위 배경음을 비동기화, 루프로 출력한다.
void Random_music(void)
{
	int music_number; // 출력한 음악의 번호
	int total_music = 9; // 출력 가능한 랜덤 음악 개수(default 제외)
	
	do
	{
		music_number = (rand() % total_music) + 1; // 범위: 1~(total_music)

		switch (music_number)
		{
		case 1:
			if (play_record == 1)
			{
				whether_repeated = 1;
				break;
			}
			else
			{
				play_record = 1;
				whether_repeated = 0;
				PlaySound(TEXT("music_number1.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			}			
		case 2:
			if (play_record == 2)
			{
				whether_repeated = 1;
				break;
			}
			else
			{
				play_record = 2;
				whether_repeated = 0;
				PlaySound(TEXT("music_number2.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			}
		case 3:
			if (play_record == 3)
			{
				whether_repeated = 1;
				break;
			}
			else
			{
				play_record = 3;
				whether_repeated = 0;
				PlaySound(TEXT("music_number3.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			}
		case 4:
			if (play_record == 4)
			{
				whether_repeated = 1;
				break;
			}
			else
			{
				play_record = 4;
				whether_repeated = 0;
				PlaySound(TEXT("music_number4.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			}
		case 5:
			if (play_record == 5)
			{
				whether_repeated = 1;
				break;
			}
			else
			{
				play_record = 5;
				whether_repeated = 0;
				PlaySound(TEXT("music_number5.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			}
		case 6:
			if (play_record == 6)
			{
				whether_repeated = 1;
				break;
			}
			else
			{
				play_record = 6;
				whether_repeated = 0;
				PlaySound(TEXT("music_number6.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			}
		case 7:
			if (play_record == 7)
			{
				whether_repeated = 1;
				break;
			}
			else
			{
				play_record = 7;
				whether_repeated = 0;
				PlaySound(TEXT("music_number7.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			}
		case 8:
			if (play_record == 8)
			{
				whether_repeated = 1;
				break;
			}
			else
			{
				play_record = 8;
				whether_repeated = 0;
				PlaySound(TEXT("music_number8.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			}
		case 9:
			if (play_record == 9)
			{
				whether_repeated = 1;
				break;
			}
			else
			{
				play_record = 9;
				whether_repeated = 0;
				PlaySound(TEXT("music_number9.wav"), NULL, SND_ASYNC | SND_LOOP | SND_NODEFAULT);
				break;
			}
		default:
			break;
		}
	} while (whether_repeated);
}