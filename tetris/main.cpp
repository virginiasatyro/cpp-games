//#define OLC_PGE_APPLICATION
//#include "../commonlib/olcPixelGameEngine.h"
#include <iostream>
#include <windows.h>

// command prompt   g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

/*
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		sAppName = "Television Crazy Pixels";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
        
		// called once per frame
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
                // call randon pixels
				Draw(x, y, olc::Pixel(rand() % 255, rand() % 255, rand()% 255));	
		return true;
	}
};


int main()
{
	Example demo;
    // construct screen
	if (demo.Construct(512, 480, 2, 2))
		demo.Start();

	return 0;
}
*/


std::string tetromino[7]; // 7 types
int fieldWidth = 12;
int fieldHeight = 18;
unsigned char *field = nullptr; // store elements of the fields

int screenWidth = 80;  //columns
int screenHeight = 30; // rows

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
int Rotate(int x, int y, int r)
{
	switch (r % 4)
	{
	case 0:
		return y * 4 + x;
	case 1:
		return 12 + y - (x * 4);
	case 2:
		return 15 - (y * 4) - x;
	case 3:
		return 3 - y + (x * 4);
	}
	return 0;
}

int main()
{
	// create assets
	// single dimension array to represent multiple dimension (x, y)
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");
	tetromino[0].append(L"..X.");

	tetromino[1].append(L"..X.");
	tetromino[1].append(L".XX.");
	tetromino[1].append(L".X..");
	tetromino[1].append(L"....");

	tetromino[2].append(L".X..");
	tetromino[2].append(L".XX.");
	tetromino[2].append(L"..X.");
	tetromino[2].append(L"....");

	tetromino[3].append(L"....");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L".XX.");
	tetromino[3].append(L"....");

	tetromino[4].append(L"..X.");
	tetromino[4].append(L".XX.");
	tetromino[4].append(L"..X.");
	tetromino[4].append(L"....");

	tetromino[5].append(L"....");
	tetromino[5].append(L".XX.");
	tetromino[5].append(L"..X.");
	tetromino[5].append(L"..X.");

	tetromino[6].append(L"....");
	tetromino[6].append(L".XX.");
	tetromino[6].append(L".X..");
	tetromino[6].append(L".X..");

	field = new unsigned char[fieldWidth * fieldHeight];
	for (int x = 0; x < fieldWidth; x++) // board boundary
	{
		for (int y = 0; y < fieldHeight; y++)
		{
			// make everything 0, unless the boards
			// 9 represents the border
			field[y * fieldWidth + x] = (x == 0 || (x == fieldWidth - 1) || (y = fieldHeight - 1)) ? 9 : 0;
		}
	}

	wchar_t *screen = new wchar_t[screenWidth * screenWidth];
	for (int i = 0; i < screenWidth * screenWidth; i++) screen[i] = L' ';
	HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(console);
	DWORD bytesWritten = 0;

	// display frame
	WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, {0, 0}, &bytesWritten);
	return 0;
}


// https://www.youtube.com/watch?v=jnI1gMxtrB4