#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#define ENTER 13
#define ESC 27
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77

FILE *save;
HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD saved_attributes;

int menu();
void drawBoard(int X, int Y);
void runGame();

int board[15][15];
int player = 0;
int comp = 0;
int main()
{
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	/* Save current attributes */
	GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
	saved_attributes = consoleInfo.wAttributes;

	switch (menu())
	{
	case 0:
		runGame();
		break;
	case 1:
		system("cls");
		printf("Loading\n");
		Sleep(2000);
		if (!(save = fopen("save.txt", "r")))
		{
			printf("Error opening file!\n");
			Sleep(2000);
			exit(-1);
		}
		int i, j;
		for (i = 0; i < 16; i++)
			for (j = 0; j < 16; j++)
				fscanf(save, "%d", &board[i][j]);
		fscanf(save, "%d", &player);
		fscanf(save, "%d", &comp);
		fclose(save);
		Sleep(2000);
		runGame();
		break;
	case 2:
		comp = 1;
		runGame();
		break;
	case 3:
		return 0;
	default:
		break;
	}

	return 0;
}

int menu()
{
	int i, iter = 0, key = 0;
	char *tab[4];
	tab[0] = "\tNew game\t\t";
	tab[1] = "\tLoad game\t\t";
	tab[2] = "\tvs COMPUTER\t\t";
	tab[3] = "\tEXIT\t\t\t";

	printf("---------------------------------\n");
	printf("|\tMENU\t\t\t|\n");
	printf("---------------------------------\n");

	for (i = 0; i < 4; i++)
	{
		printf("|");
		if (iter == i) SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		printf(tab[i]);
		if (iter == i) SetConsoleTextAttribute(hConsole, saved_attributes);
		printf("|\n");
	}
	printf("---------------------------------\n");
	printf("|\Control: arrows\t|\n|\Confirmationt: ENTER\t|\n");
	printf("---------------------------------\n");
	while (1)
	{
		if (_kbhit())
		{
			switch (_getch())
			{
			case UP:
				iter--;
				if (iter < 0) iter = 3;
				break;
			case DOWN:
				iter++;
				iter = iter % 4;
				break;
			case ENTER:
				return iter;
			default:
				break;
			}

			system("cls");
			printf("---------------------------------\n");
			printf("|\tMENU\t\t\t|\n");
			printf("---------------------------------\n");
			for (i = 0; i < 4; i++)
			{
				printf("|");
				if (iter == i) SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				printf(tab[i]);
				if (iter == i) SetConsoleTextAttribute(hConsole, saved_attributes);
				printf("|\n");
			}
			printf("---------------------------------\n");
			printf("|\Controls: arrows\t|\n|\Confirmation: ENTER\t|\n");
			printf("---------------------------------\n");
		}
	}
}

void drawBoard(int X, int Y)
{
	int i, j;
	for (i = 0; i < 15; i++)
	{
		printf("|");
		for (j = 0; j < 59; j++) printf("-");
		printf("|\n| ");

		for (j = 0; j < 15; j++)
		{
			if (X == j && Y == i)
			{
				if (board[Y][X] != ' ') SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				printf("%c", 254);
				if (board[Y][X] != ' ') SetConsoleTextAttribute(hConsole, saved_attributes);
				printf(" | ");
			}
			else
			{
				switch (board[i][j])
				{
				case 'X':
					SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
					break;
				case 'O':
					SetConsoleTextAttribute(hConsole, 6);
					break;
				default:
					break;
				}
				printf("%c", board[i][j]);
				SetConsoleTextAttribute(hConsole, saved_attributes);
				printf(" | ");
			}
		}
		printf("\n");
	}

	printf("|");
	for (j = 0; j < 59; j++) printf("-");
	printf("|\n");
}

void runGame()
{
	int i, j;
	for (i = 0; i < 15; i++)
		for (j = 0; j < 15; j++)
			if (board[i][j] != 'X' && board[i][j] != 'O') board[i][j] = ' ';

	int X = 0, Y = 0;

	system("cls");
	printf("Next move: ");
	if (player) printf("X\n\n");
	else printf("O\n");
	drawBoard(0, 0, 0);
	printf("\Controls: arrows\nConfirmation: ENTER\nSave game: ESC\nExit: Alt + F4\n");
	int c0 = 0, c45 = 0, c90 = 0, c135 = 0, c180 = 0, c225 = 0, c270 = 0, c315 = 0;
	int tempX, tempY;
	while (1)
	{
		if (_kbhit())
		{

			switch (_getch())
			{
			case UP:
				Y--;
				if (Y < 0) Y = 14;
				break;
			case DOWN:
				Y++;
				if (Y > 14) Y = 0;
				break;
			case LEFT:
				X--;
				if (X < 0) X = 14;
				break;
			case RIGHT:
				X++;
				if (X > 14) X = 0;
				break;
			case ENTER:
				if (player)
				{
					if (board[Y][X] == ' ')
					{
						board[Y][X] = 'X';
						player--;

						tempX = X; tempY = Y;
						while (board[tempY][(tempX++ % 14)] == 'X') c0++;

						tempX = X; tempY = Y;
						while (board[((tempY--) % 14 + 14) % 14][(tempX++ % 14)] == 'X') c45++;

						tempX = X; tempY = Y;
						while (board[((tempY--) % 14 + 14) % 14][tempX] == 'X') c90++;

						tempX = X; tempY = Y;
						while (board[((tempY--) % 14 + 14) % 14][((tempX--) % 14 + 14) % 14] == 'X') c135++;

						tempX = X; tempY = Y;
						while (board[tempY][((tempX--) % 14 + 14) % 14] == 'X') c180++;

						tempX = X; tempY = Y;
						while (board[(tempY++ % 14)][((tempX--) % 14 + 14) % 14] == 'X') c225++;

						tempX = X; tempY = Y;
						while (board[(tempY++ % 14)][tempX] == 'X') c270++;

						tempX = X; tempY = Y;
						while (board[(tempY++ % 14)][(tempX++ % 14)] == 'X') c315++;

						if ((c0 + c180) == 6 || (c45 + c225) == 6 || (c90 + c270) == 6 || (c135 + c315) == 6)
						{
							Sleep(2000);
							system("cls");
							printf("Winner: X");
							Sleep(5000);
							return;
						}
						else
						{
							c0 = 0, c45 = 0, c90 = 0, c135 = 0, c180 = 0, c225 = 0, c270 = 0, c315 = 0;
						}
					}
				}
				else
				{
					if (board[Y][X] == ' ')
					{
						board[Y][X] = 'O';
						player++;

						tempX = X; tempY = Y;
						while (board[tempY][(tempX++ % 14)] == 'O') c0++;

						tempX = X; tempY = Y;
						while (board[((tempY--) % 14 + 14) % 14][(tempX++ % 14)] == 'O') c45++;

						tempX = X; tempY = Y;
						while (board[((tempY--) % 14 + 14) % 14][tempX] == 'O') c90++;

						tempX = X; tempY = Y;
						while (board[((tempY--) % 14 + 14) % 14][((tempX--) % 14 + 14) % 14] == 'O') c135++;

						tempX = X; tempY = Y;
						while (board[tempY][((tempX--) % 14 + 14) % 14] == 'O') c180++;

						tempX = X; tempY = Y;
						while (board[(tempY++ % 14)][((tempX--) % 14 + 14) % 14] == 'O') c225++;

						tempX = X; tempY = Y;
						while (board[(tempY++ % 14)][tempX] == 'O') c270++;

						tempX = X; tempY = Y;
						while (board[(tempY++ % 14)][(tempX++ % 14)] == 'O') c315++;

						if ((c0 + c180) == 6 || (c45 + c225) == 6 || (c90 + c270) == 6 || (c135 + c315) == 6)
						{
							Sleep(2000);
							system("cls");
							printf("Winner: O");
							Sleep(5000);
							return 0;
						}
						else
						{
							c0 = 0, c45 = 0, c90 = 0, c135 = 0, c180 = 0, c225 = 0, c270 = 0, c315 = 0;
						}

						if (comp)
						{
							do
							{
								X += -3 + rand() % 5;
								Y += -3 + rand() % 5;
								if (X < 0) X = 0;
								if (X > 15) X = 15;
								if (Y < 0) Y = 0;
								if (Y > 15) Y = 15;
							} while (board[Y][X] != ' ');
							board[Y][X] = 'X';
							player--;

							tempX = X; tempY = Y;
							while (board[tempY][(tempX++ % 14)] == 'X') c0++;

							tempX = X; tempY = Y;
							while (board[((tempY--) % 14 + 14) % 14][(tempX++ % 14)] == 'X') c45++;

							tempX = X; tempY = Y;
							while (board[((tempY--) % 14 + 14) % 14][tempX] == 'X') c90++;

							tempX = X; tempY = Y;
							while (board[((tempY--) % 14 + 14) % 14][((tempX--) % 14 + 14) % 14] == 'X') c135++;

							tempX = X; tempY = Y;
							while (board[tempY][((tempX--) % 14 + 14) % 14] == 'X') c180++;

							tempX = X; tempY = Y;
							while (board[(tempY++ % 14)][((tempX--) % 14 + 14) % 14] == 'X') c225++;

							tempX = X; tempY = Y;
							while (board[(tempY++ % 14)][tempX] == 'X') c270++;

							tempX = X; tempY = Y;
							while (board[(tempY++ % 14)][(tempX++ % 14)] == 'X') c315++;

							if ((c0 + c180) == 6 || (c45 + c225) == 6 || (c90 + c270) == 6 || (c135 + c315) == 6)
							{
								Sleep(2000);
								system("cls");
								printf("Winner: X");
								Sleep(5000);
								return;
							}
							else
							{
								c0 = 0, c45 = 0, c90 = 0, c135 = 0, c180 = 0, c225 = 0, c270 = 0, c315 = 0;
							}

						}
					}
				}


				break;
			case ESC:
				system("cls");
				if (save = fopen("save.txt", "w"))
					printf("Saved!\n");
				int i, j;
				for (i = 0; i < 16; i++)
					for (j = 0; j < 16; j++)
						fprintf(save, "%d ", board[i][j]);
				fprintf(save, "%d ", player);
				fprintf(save, "%d ", comp);
				fclose(save);
				Sleep(2000);
				return;
			default:
				break;
			}


			int end = 1;
			for (i = 0; i < 15; i++)
				for (j = 0; j < 15; j++)
					if (board[i][j] == ' ') end = 0;
			if (end)
			{
				printf("DRAW");
				Sleep(3000);
				return;
			}


			system("cls");
			printf("Move: ");
			if (player) printf("X\n");
			else printf("O\n");
			drawBoard(X, Y);
			printf("\Controls: arrows\nConfirmation: ENTER\nSave game: ESC\nExit: Alt + F4\n");
		}
	}
}
