/*
	TETRIS

	- Based on https://www.youtube.com/watch?v=8OK8_tHeCIA - for learning purposes;

	- Based on: https://github.com/OneLoneCoder/Javidx9/blob/master/SimplyCode/OneLoneCoder_Tetris.cpp

	- Run: g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
	       g++ -o main.exe main.cpp -static -std=c++17

	- Game timing - since computers aren't the same, we need a way to make sure they are going to work similarly

	- In pomputing top-left is always {0, 0};

*/

#ifndef UNICODE // https://stackoverflow.com/questions/13977388/error-cannot-convert-const-wchar-t-13-to-lpcstr-aka-const-char-in-assi
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <iostream>
#include <windows.h>
#include <string>
#include <thread>
#include <chrono>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

constexpr int screenWidth = 80;
constexpr int screenHeight = 30;

constexpr int fieldWidth = 12;
constexpr int fieldHeight = 18;

constexpr int tetroSize = 4;

std::wstring TETROMINO[7]; // 7 types

unsigned char *field = nullptr; // store elements of the fields

/* this means (x and y size is 16 - width 4)
 * locate: i = y * w + x -> at 0 degree rotation
 * -> 90 degree rotation i = 12 + y - (x * 4)
 * -> 180 degree rotation i = 15 - (y * 4) - x
 * -> 270 degree rotation i = 3 + y + (x * 4)
 * inputs:
 * x: position x
 * y: position y
 * r: 0, 1, 2, 3 (0, 90, 180, 270)
 */
int Rotate(int x, int y, int rot)
{
	int pi = 0;
	switch (rot % 4)
	{
	case 0:
		pi =  y * tetroSize + x;       // 0 degrees
		break;
	case 1:
		pi = 12 + y - (x * tetroSize); // 90 degrees
		break;
	case 2:
		pi = 15 - (y * tetroSize) - x; // 180 degrees
		break;
	case 3:
		pi = 3 - y + (x * tetroSize);  // 270 degrees
		break;
	default:
		pi = 0;
	}
	return pi;
}

bool doesPieceFit(int tetromino, int rot, int posX, int posY)
{
	for (int x = 0; x < tetroSize; x++)
	{
		for (int y = 0; y < tetroSize; y++)
		{
			// get index into piece
			int pi = Rotate(x, y, rot);

			// get index into field
			int fi = (posY + y) * fieldWidth + (posX + x);

			if (posX + x >= 0 && posX + x < fieldWidth)
			{
				if (posY + y >= 0 && posY + y < fieldHeight)
				{
					if (TETROMINO[tetromino][pi] != L'.' && field[fi] != 0)
					{
						return false; // colision detected
					}
				}
			}
		}
	}
	return true;
}

int main()
{
	// create screen buffer
	wchar_t *screen = new wchar_t[screenWidth * screenWidth];
	for (int i = 0; i < screenWidth * screenWidth; i++)
	{
		screen[i] = L' ';
	}
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	DWORD bytesWritten = 0;

	// create assets
	// single dimension array to represent multiple dimension (x, y)
	// . - empty space, X - TETROMINO
	/*TETROMINO[0].append(L"..X.");
	TETROMINO[0].append(L"..X.");
	TETROMINO[0].append(L"..X.");
	TETROMINO[0].append(L"..X.");

	TETROMINO[1].append(L"..X.");
	TETROMINO[1].append(L".XX.");
	TETROMINO[1].append(L".X..");
	TETROMINO[1].append(L"....");

	TETROMINO[2].append(L".X..");
	TETROMINO[2].append(L".XX.");
	TETROMINO[2].append(L"..X.");
	TETROMINO[2].append(L"....");

	TETROMINO[3].append(L"....");
	TETROMINO[3].append(L".XX.");
	TETROMINO[3].append(L".XX.");
	TETROMINO[3].append(L"....");

	TETROMINO[4].append(L"..X.");
	TETROMINO[4].append(L".XX.");
	TETROMINO[4].append(L"..X.");
	TETROMINO[4].append(L"....");

	TETROMINO[5].append(L"....");
	TETROMINO[5].append(L".XX.");
	TETROMINO[5].append(L"..X.");
	TETROMINO[5].append(L"..X.");

	TETROMINO[6].append(L"....");
	TETROMINO[6].append(L".XX.");
	TETROMINO[6].append(L".X..");
	TETROMINO[6].append(L".X..");*/

	TETROMINO[0].append(L"..X...X...X...X."); // Tetronimos 4x4
	TETROMINO[1].append(L"..X..XX...X.....");
	TETROMINO[2].append(L".....XX..XX.....");
	TETROMINO[3].append(L"..X..XX..X......");
	TETROMINO[4].append(L".X...XX...X.....");
	TETROMINO[5].append(L".X...X...XX.....");
	TETROMINO[6].append(L"..X...X..XX.....");

	// Create play field
	field = new unsigned char[fieldWidth * fieldHeight];
	for (int x = 0; x < fieldWidth; x++) // board boundary
	{
		for (int y = 0; y < fieldHeight; y++)
		{
			// make everything 0, unless the boards
			// 9 represents the border
			field[y * fieldWidth + x] = ((x == 0) || (x == fieldWidth - 1) || (y == fieldHeight - 1)) ? 9 : 0;
		}
	}

	bool gameOver = false;

	int currentPiece = 0;
	int currentRotation = 0;
	int currentX = fieldWidth / 2;
	int currentY = 0;

	bool key[4];

	while (!gameOver)
	{
		// GAME TIMING ----------------------------------------------------------------------------------------------
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

		// INPUT ----------------------------------------------------------------------------------------------------
		
		for(int k = 0; k < 4; k++)
		{                                                       // R   L  D Z
			key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
		}

		// GAME LOGIC -----------------------------------------------------------------------------------------------
	    
		currentX += (key[0] && doesPieceFit(currentPiece, currentRotation, currentX + 1, currentY)) ? 1 : 0;
		currentX -= (key[1] && doesPieceFit(currentPiece, currentRotation, currentX - 1, currentY)) ? 1 : 0;		
		currentY += (key[2] && doesPieceFit(currentPiece, currentRotation, currentX, currentY + 1)) ? 1 : 0;

		// RENDER OUTPUT --------------------------------------------------------------------------------------------

		// Draw field
		for (int x = 0; x < fieldWidth; x++)
		{
			for (int y = 0; y < fieldHeight; y++)
			{
				screen[(y + 2) * screenWidth + (x + 2)] = L" ABCDEFG=#"[field[y * fieldWidth + x]];
			}
		}

		// Draw current piece
		for (int x = 0; x < tetroSize; x++)
		{
			for (int y = 0; y < tetroSize; y++)
			{
				if (TETROMINO[currentPiece][Rotate(x, y, currentRotation)] != L'.')
				{
					// + 65 -> it mean some value on ASCII tale - A, B, C,..
					screen[(currentY + y + 2) * screenWidth + (currentX + x + 2)] = currentPiece + 65;
				}
			}
		}

		// display frame - function that permits use of prompt as screen buffer effectively
		// std::cout << "WriteConsoleOutputCharacter";
		WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, {0, 0}, &bytesWritten);
	}
	CloseHandle(console);
	std::cout << "Game Over";
	system("pause");
	return 0;
}

// https://www.youtube.com/watch?v=jnI1gMxtrB4