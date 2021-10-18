#define _CRT_SECURE_NO_WARNING 1
#include<stdio.h>
#include<windows.h>
#include<conio.h>
#include<time.h>

#define screen_x 80
#define screen_y 30
HANDLE wHnd;
COORD bufferSize = { screen_x,screen_y };
SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };
int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void setcolor(int fg, int bg) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void erase_ship(int x, int y)
{
	setcolor(0, 0);
	gotoxy(x, y);
	printf("       ");
}

void erase_bg(int x, int y)
{
	setcolor(0, 0);
	gotoxy(x, y);
	printf("       ");
}

void draw_ship(int x, int y)
{
	setcolor(3, 5);
	gotoxy(x, y);
	printf(" <-0-> ");
}

void draw_gunshot(int x, int y) {
	setcolor(3, 0);
	gotoxy(x, y);
	printf("^");
}

void erase_gunshot(int x, int y) {
	setcolor(0, 0);
	gotoxy(x, y);
	printf(" ");
}

struct Gunshot {
	int active = 0;
	int x = 0; int y = 0;
};

void draw_star(int x, int y)
{
	setcolor(6, 0);
	gotoxy(x, y);
	printf("*");
}

void erase_star(int x, int y) {
	setcolor(0, 0);
	gotoxy(x, y);
	printf(" ");
}

void scorepoint(int score) {
	setcolor(7, 0);
	gotoxy(65, 0);
	printf("score : %d",score);
}

//เช็คตำแหน่ง
char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; COORD c = { x,y }; DWORD num_read;
	if (
		!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))

		return '\0';
	else
		return buf[0];

}


int main()
{
	setConsole(screen_x, screen_y);
	setcursor(0);
	int i;
	srand(time(NULL));
	for (i = 0; i < 20; i++) {
		draw_star(10 + rand() % 61, 2 + rand() % 5);
	}

	Gunshot gunshots[5];
	char ch = ' ';
	int x = 38, y = 29;
	int press = 0;

	int score = 0;
	scorepoint(score);

	draw_ship(x, y);
	do {
		if (_kbhit()) {
			ch = _getch();
			if (ch == 'a' && x > 0) {
				press = 1;
			}
			if (ch == 'd' && x < 73) {
				press = 2;
			}

			if (ch == 's') {
				press = 3;
			}
			
			if (ch == ' ')
			{	//Beep(500, 125);
				for (int i = 0; i < 5; i++)
				{
					if (gunshots[i].active == 0) {
						gunshots[i].active = 1;
						gunshots[i].x = x + 3;
						gunshots[i].y = y;
						break;
					}
				}
			}

			fflush(stdin);
		}
		for (int i = 0; i < 5; i++)
		{
			if (gunshots[i].active == 1) {
				erase_gunshot(gunshots[i].x, gunshots[i].y);
				if (cursor (gunshots[i].x, gunshots[i].y-1) == '*')
				{
					gunshots[i].active = 0;
					Beep(550, 150);
					erase_star(gunshots[i].x, gunshots[i].y-1); //erase star
					draw_star(10 + rand() % 61, 2 + rand() % 5); //new star
					score = score + 100;
					scorepoint(score);
				}
				else if (gunshots[i].y > 0 )
				{
					draw_gunshot(gunshots[i].x, --gunshots[i].y);
				}
				else {
					gunshots[i].active = 0;
				}
			}
		}

			if (press == 1 && x > 0)
			{
				erase_ship(x, y);
				erase_bg(x, y);
				draw_ship(--x, y);
			}
			if (press == 2 && x < 73)
			{
				erase_ship(x, y);
				erase_bg(x, y);
				draw_ship(++x, y);
			}
			if (press == 3) {
				erase_ship(x, y);
				erase_bg(x, y);
				draw_ship(x, y);
			}
		Sleep(100);
	} while (ch != 'x');

	return 0;
}
