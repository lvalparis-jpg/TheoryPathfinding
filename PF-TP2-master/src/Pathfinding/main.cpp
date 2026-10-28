#include <iostream>
#include <conio.h>

#define NOMINMAX
#include <windows.h>

#define BG_RED "\033[41m"
#define BG_GREEN "\033[42m"
#define DEFAULT "\033[0m"

#define ENTER 13

constexpr int ROWS = 10;
constexpr int COLS = 10;

void gotoxy(short x, short y) 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD position = { x, y };
	SetConsoleCursorPosition(hConsole, position);
}

void hidecursor()
{
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void Refresh() 
{
	gotoxy(0, 0);
}

class Grid 
{
	int playerRow = 0;
	int playerCol = 0;

	int cursorRow = 0;
	int cursorCol = 0;

	bool isInPath[ROWS][COLS];

public:
	void Run()
	{
		ResetPath();

		while (true) 
		{
			Display();
			Input();
			Path();

			Refresh();
		}
	}

	void ResetPath() 
	{
		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				isInPath[row][col] = false;
			}
		}
	}

	void Path() 
	{	
		ResetPath();

		int diffRow = cursorRow - playerRow;
		int lenghtRow = std::abs(diffRow);
		if (lenghtRow != 0) 
		{
			int dirRow = diffRow / lenghtRow;
			for (int row = 0; row < lenghtRow; row++)
			{
				isInPath[playerRow + dirRow * row][playerCol] = true;
			}
		}

		int diffCol = cursorCol - playerCol;
		int lenghtCol = std::abs(diffCol);
		if (lenghtCol != 0) 
		{
			int dirCol = diffCol / lenghtCol;
			for (int col = 0; col < lenghtCol; col++)
			{
				isInPath[playerRow + diffRow][playerCol + dirCol * col] = true;
			}
		}
	}


	void Display()
	{
		//Display
		for (int row = 0; row < ROWS; row++)
		{
			for (int col = 0; col < COLS; col++)
			{
				std::cout << "[";

				if (cursorRow == row && cursorCol == col)
				{
					std::cout << BG_GREEN;
				}
				else if (isInPath[row][col])
				{
					std::cout << BG_RED;
				}

				if (playerRow == row && playerCol == col)
				{
					std::cout << "O";
				}

				else
				{
					std::cout << " ";
				}

				std::cout << DEFAULT;

				std::cout << "]";
			}

			std::cout << std::endl;
		}
	}

	void Input()
	{
		while (true) 
		{
			int input = _getch();
			switch (input)
			{
			case 'z':
				cursorRow = std::max(0, cursorRow - 1);
				return;

			case 's':
				cursorRow = std::min(ROWS - 1, cursorRow + 1);
				return;

			case 'q':
				cursorCol = std::max(0, cursorCol - 1);
				return;

			case 'd':
				cursorCol = std::min(COLS - 1, cursorCol + 1);
				return;

			case '\r':
				playerCol = cursorCol;
				playerRow = cursorRow;
				return;
			}

		}
		
		system("cls");
	}
};

int main()
{
	hidecursor();

	Grid grid;

	grid.Run();

	return 0;
}
