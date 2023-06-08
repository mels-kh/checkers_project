#include <iostream>
#include <vector>
#include <stdio.h>
#include "graphics.h"

#pragma comment(lib,"graphics.lib")
int board[8][8] = {
	{1, 0, 1, 0, 1, 0, 1, 0},
	{0, 1, 0, 1, 0, 1, 0, 1},
	{1, 0, 1, 0, 1, 0, 1, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, -1, 0, -1, 0, -1, 0, -1},
	{-1, 0, -1, 0, -1, 0, -1, 0},
	{0, -1, 0, -1, 0, -1, 0, -1}

	/*{-7, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 7, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0}*/
};
int turn = 1;//1 = white, -1 = black
int queen = 7;
int last_x = -1, last_y = -1, game_state = 2;
bool eating = false, canTake = false;
std::vector<std::vector<int>> moves;
int sign(int x)
{
	if (x == 0)
		return 0;
	return x / abs(x);
}
void print_board()
{
	cleardevice();
	settextstyle(3, HORIZ_DIR, 3);
	setcolor(WHITE);
	if (game_state == 2)
		outtextxy(40, 805, turn == 1 ? (char*)"White to move" : (char*)"Black to move");
	else if (game_state == 1)
		outtextxy(40, 805, (char*)"White Won!");
	else if (game_state == -1)
		outtextxy(40, 805, (char*)"Black Won!");
	else if (game_state == 0)
		outtextxy(40, 805, (char*)"Draw!");
	char str[10];
	int w = 0, b = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] > 0)
				w++;
			else if (board[i][j] < 0)
				b++;
		}
	}
	w = 12 - w;
	b = 12 - b;
	settextstyle(3, HORIZ_DIR, 4);
	sprintf_s(str, "%d", w);
	outtextxy(810, 5, str);
	sprintf_s(str, "%d", b);
	outtextxy(810, 770, str);

	if (game_state == 2)
	{
		setcolor(turn == -1 ? WHITE : BLACK);
		setfillstyle(SOLID_FILL, turn == 1 ? WHITE : BLACK);
	}
	else
	{
		setcolor(turn == 1 ? WHITE : BLACK);
		setfillstyle(SOLID_FILL, turn == -1 ? WHITE : BLACK);
	}
	bar(5, 805, 35, 835);

	rectangle(5, 805, 35, 835);

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			setfillstyle(SOLID_FILL, (i % 2 != j % 2) ? COLOR(77, 47, 41) : WHITE);
			bar(j * 100, i * 100, (j + 1) * 100, (i + 1) * 100);
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[j][i] == 0) continue;
			else if (board[j][i] > 0)
			{
				setcolor(BLACK);
				setfillstyle(SOLID_FILL, COLOR(200, 200, 200));
			}
			else
			{
				setcolor(WHITE);
				setfillstyle(SOLID_FILL, BLACK);
			}
			fillellipse(i * 100 + 50, (7 - j) * 100 + 50, 40, 40);
			if (abs(board[j][i]) == queen)
			{
				int points[10] = { i * 100 + 20,(7 - j) * 100 + 50,
					i * 100 + 50,(8 - j) * 100 - 20,
					(i + 1) * 100 - 20,(7 - j) * 100 + 50,
					i * 100 + 50, (7 - j) * 100 + 20,
					 i * 100 + 20, (7 - j) * 100 + 50 };
				setfillstyle(SOLID_FILL, board[j][i] < 0 ? RED : BLUE);
				fillpoly(5, points);

			}
		}
	}
}
bool in_board(int sx, int sy, int ex, int ey)
{
	return sx >= 0 && sx < 8 && sy >= 0 && sy < 8 && ex >= 0 && ex < 8 && ey >= 0 && ey < 8;
}
bool on_black(int ex, int ey)
{
	return ex % 2 == ey % 2;
}
bool empty(int x, int y)
{
	return board[y][x] == 0;
}
bool possible_move(int sx, int sy, int ex, int ey)
{
	if (last_x != -1 && (last_x != sx || last_y != sy))
		return false;
	eating = false;
	if (abs(board[sy][sx]) == 1)
	{
		if (abs(sx - ex) == 1 && ey - sy == turn)
			return true;
		if (abs(sx - ex) == 2 && abs(ey - sy) == 2 && sign(board[(sy + ey) / 2][(sx + ex) / 2]) == turn * -1)
		{
			eating = true;
			return true;
		}
		return false;
	}
	else
	{
		if (abs(ex - sx) != abs(ey - sy))
			return false;
		if (abs(ex - sx) == 1)
		{
			return board[ey][ex] == 0;
		}
		if (abs(ex - sx) == 2)
		{
			eating = sign(board[(ey + sy) / 2][(ex + sx) / 2]) == turn * -1;
			return board[ey][ex] == 0 && sign(board[(ey + sy) / 2][(ex + sx) / 2]) != turn;
		}
		int dx = sign(ex - sx);
		int dy = sign(ey - sy);
		int other = 0;
		for (int i = 1; sy + i * dy != ey; i++)
		{
			int s = sign(board[sy + i * dy][sx + i * dx]);
			if (s == 0)
				continue;
			if (s == turn)
				return false;
			else
			{
				other++;
				if (other > 1)
					return false;
			}
		}
		if (other == 1)
			eating = true;
		return true;
	}
}
bool can_take(int sx, int ex, int sy, int ey)
{
	if (!in_board(sx, sy, ex, ey) || !on_black(ex, ey) || !empty(ex, ey) ||
		sign(board[sy][sx]) != turn || !possible_move(sx, sy, ex, ey))
		return false;
	if (abs(board[sy][sx]) == 1)
		return abs(sx - ex) == 2 && abs(ey - sy) == 2 && sign(board[(sy + ey) / 2][(sx + ex) / 2]) == turn * -1;
	else
	{
		if (board[ey][ex] != 0)
			return false;
		if (abs(ex - sx) != abs(ey - sy))
			return false;
		if (abs(ex - sx) == 1)
			return false;
		if (abs(ex - sx) == 2)
		{
			return board[ey][ex] == 0 && sign(board[(ey + sy) / 2][(ex + sx) / 2]) == turn * -1;
		}
		int dx = sign(ex - sx);
		int dy = sign(ey - sy);
		int other = 0;
		for (int i = 1; sy + i * dy != ey; i++)
		{
			int s = sign(board[sy + i * dy][sx + i * dx]);
			if (s == 0)
				continue;
			if (s == turn)
				return false;
			else
			{
				other++;
				if (other > 1)
					return false;
			}
		}
		return other == 1;
	}
}
bool can_take()
{
	if (last_x != -1)
	{
		int i = last_x, j = last_y;
		if (abs(board[j][i] == 1))
		{
			return (can_take(i, i + 2, j, j + 2) || can_take(i, i + 2, j, j - 2) ||
				can_take(i, i - 2, j, j + 2) || can_take(i, i - 2, j, j - 2));
		}
		else
		{
			for (int dx = -1; dx <= 1; dx += 2)
			{
				for (int dy = -1; dy <= 1; dy += 2)
				{
					for (int k = 2; in_board(i, j, k * dx + i, k * dy + j); k++)
					{
						if (can_take(i, k * dx + i, j, k * dy + j))
							return true;
					}
				}
			}
			return false;
		}
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (sign(board[j][i]) != turn)
				continue;
			if (abs(board[j][i]) == 1)
			{
				if (can_take(i, i + 2, j, j + 2) || can_take(i, i + 2, j, j - 2) ||
					can_take(i, i - 2, j, j + 2) || can_take(i, i - 2, j, j - 2))
				{
					return true;
				}
			}
			else
			{
				for (int dx = -1; dx <= 1; dx += 2)
				{
					for (int dy = -1; dy <= 1; dy += 2)
					{
						for (int k = 2; in_board(i, j, k * dx + i, k * dy + j); k++)
						{
							if (can_take(i, k * dx + i, j, k * dy + j))
								return true;
						}
					}
				}
				//return false;
			}
		}
	}
	return false;
}
int game_ended()//+1 = white won, 0 = draw, -1 = black won, 2 = not ended
{
	bool foundWhite = false, foundBlack = false;
	for (int i = 0; i < 8; i++)
	{
		int j = 0;
		for (j = 0; j < 8; j++)
		{
			int square = board[i][j];
			if (square == 0) continue;
			if (square > 0) foundWhite = true;
			if (square < 0) foundBlack = true;
			if (foundWhite && foundBlack)
			{
				break;
			}
		}
		if (j < 8)
			break;
	}
	if (!foundWhite)
		return -1;
	else if (!foundBlack)
		return 1;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board[i][j] * turn <= 0) continue;
			if ((possible_move(j, i, j + 1, i + 1) && in_board(j, i, j + 1, i + 1) && empty(j + 1, i + 1)) ||
				(possible_move(j, i, j - 1, i + 1) && in_board(j, i, j - 1, i + 1) && empty(j - 1, i + 1)) ||
				(possible_move(j, i, j + 1, i - 1) && in_board(j, i, j + 1, i - 1) && empty(j + 1, i - 1)) ||
				(possible_move(j, i, j - 1, i - 1) && in_board(j, i, j - 1, i - 1) && empty(j - 1, i - 1)) ||
				(possible_move(j, i, j + 2, i + 2) && in_board(j, i, j + 2, i + 2) && empty(j + 2, i + 2)) ||
				(possible_move(j, i, j - 2, i + 2) && in_board(j, i, j - 2, i + 2) && empty(j - 2, i + 2)) ||
				(possible_move(j, i, j + 2, i - 2) && in_board(j, i, j + 2, i - 2) && empty(j + 2, i - 2)) ||
				(possible_move(j, i, j - 2, i - 2) && in_board(j, i, j - 2, i - 2) && empty(j - 2, i - 2)))
			{
				return 2;
			}
		}
	}
	return 0;
}
void show_moves(int x, int y)
{
	setfillstyle(SOLID_FILL, GREEN);
	if (sign(board[y][x]) != turn)
		return;
	if (abs(board[y][x]) == 1)
	{
		for (int dx = -1; dx <= 1; dx += 2)
		{
			for (int dy = -1; dy <= 1; dy += 2)
			{
				if (in_board(x, y, x + dx, y + dy) && empty(x + dx, y + dy) &&
					possible_move(x, y, x + dx, y + dy) && eating == canTake)
				{
					bar((x + dx) * 100, (7 - y - dy) * 100, (x + dx + 1) * 100, (8 - y - dy) * 100);
				}
				if (in_board(x, y, x + dx * 2, y + dy * 2) && empty(x + dx * 2, y + dy * 2) &&
					possible_move(x, y, x + dx * 2, y + dy * 2) && eating == canTake)
				{
					bar((x + dx * 2) * 100, (7 - y - dy * 2) * 100, (x + dx * 2 + 1) * 100, (8 - y - dy * 2) * 100);
				}
			}
		}
	}
	else
	{
		for (int k = 1; k < 8; k++)
		{
			for (int dx = -1; dx <= 1; dx += 2)
			{
				for (int dy = -1; dy <= 1; dy += 2)
				{
					if (!in_board(x, y, x + dx * k, y + dy * k) || !empty(x + dx * k, y + dy * k) ||
						!possible_move(x, y, x + dx * k, y + dy * k) || eating != canTake) continue;
					bar((x + dx * k) * 100, (7 - y - dy * k) * 100, (x + dx * k + 1) * 100, (8 - y - dy * k) * 100);
				}
			}
		}
	}
}
int main()
{
	HWND window;
	AllocConsole();
	window = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(window, 1);

	int gd = DETECT, gm;
	char c = ' ';
	initgraph(&gd, &gm, &c);
	initwindow(850, 850, "Checkers");
	bool valid_move = true;
	print_board();
	canTake = can_take();
	while (true)
	{
		if (!valid_move)
		{
			//cout << "non valid move, please try again\n";
			setcolor(RED);
			settextstyle(3, HORIZ_DIR, 2);
			outtextxy(650, 805, (char*)"Non valid move!");
		}
		int sx, ex, sy, ey;

		POINT cursor;
		bool canFinish = false;
		while (true)
		{
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				GetCursorPos(&cursor);
				ScreenToClient(GetForegroundWindow(), &cursor);
				sx = cursor.x / 100;
				sy = 7 - cursor.y / 100;
				if (sx < 0 || sx > 7 || sy < 0 || sy > 7)
					continue;
				canFinish = true;
			}
			else if (canFinish)
			{
				setcolor(GREEN);
				circle(sx * 100 + 50, (7 - sy) * 100 + 50, 45);
				show_moves(sx, sy);
				break;
			}
		}
		canFinish = false;
		while (true)
		{
			if (GetAsyncKeyState(VK_LBUTTON))
			{
				GetCursorPos(&cursor);
				ScreenToClient(GetForegroundWindow(), &cursor);
				ex = cursor.x / 100;
				ey = 7 - cursor.y / 100;
				if (ex < 0 || ex > 7 || ey < 0 || ey > 7)
					continue;
				canFinish = true;
			}
			else if (canFinish)
			{
				setcolor(RED);
				circle(ex * 100 + 50, (7 - ey) * 100 + 50, 40);
				break;
			}
		}

		if (!in_board(sx, sy, ex, ey) || !on_black(ex, ey) || !empty(ex, ey) ||
			sign(board[sy][sx]) != turn || !possible_move(sx, sy, ex, ey) || eating != canTake)
		{
			valid_move = false;
			print_board();
			continue;
		}
		valid_move = true;
		std::vector<int> move;
		move.push_back(sx);
		move.push_back(sy);
		move.push_back(ex);
		move.push_back(ey);
		moves.push_back(move);
		std::swap(board[sy][sx], board[ey][ex]);
		if (eating)
		{
			if (abs(board[ey][ex]) == 1)
			{
				board[(sy + ey) / 2][(sx + ex) / 2] = 0;
			}
			else
			{
				int dx = sign(ex - sx);
				int dy = sign(ey - sy);
				for (int i = 1; sx + i * dx != ex; i++)
				{
					if (sign(board[sy + i * dy][sx + i * dx]) == turn * -1)
					{
						board[sy + i * dy][sx + i * dx] = 0;
						break;
					}
				}
			}
			last_x = ex;
			last_y = ey;
		}
		else
		{
			last_x = -1;
			last_y = -1;
		}
		if (abs(board[ey][ex]) == 1 && (turn == 1 && ey == 7 || turn == -1 && ey == 0))
		{
			board[ey][ex] *= queen;
		}
		bool was_eating = eating;
		if (eating)
			canTake = can_take();
		if (!canTake || !was_eating)
		{
			last_x = -1;
			last_y = -1;
			turn *= -1;
		}
		canTake = can_take();
		print_board();

		game_state = game_ended();
		if (game_state == 2)
			continue;
		else
		{
			if (game_state == 1)
				std::cout << "white won!";
			else if (game_state == -1)
				std::cout << "black won!";
			else if (game_state == 0)
				std::cout << "draw!";
			print_board();
			break;
		}
	}
	getch();
	closegraph();
	return 0;
}