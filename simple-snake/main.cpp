/*
    SNAKE

    - Run: g++ -o main.exe main.cpp -static -std=c++17

*/

#ifndef UNICODE // https://stackoverflow.com/questions/13977388/error-cannot-convert-const-wchar-t-13-to-lpcstr-aka-const-char-in-assi
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <iostream>
#include <windows.h>
#include <list>
#include <chrono>
#include <thread>
//#include <string>
//#include <vector>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

constexpr int screenWidth  = 120;
constexpr int screenHeight = 80;

struct snakeSegment
{
    int x;
    int y;
};

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

    // linked list - push in the head - pop in the tail
    std::list<snakeSegment> snake = {{60, 15}, {61, 15}, {62, 15}, {63, 15}, {64, 15}, {65, 15}, {66, 15}, {67, 15}, {68, 15}, {69, 15}};
    int foodX = 20;
    int foodY = 15;
    int score = 0;
    int snakeDirection = 3;
    bool snakeDead = false;
    bool keyLeft = false;
    bool keyRight = false;
    bool keyLeftOld = false;
    bool keyRightOld = false;

    // Game loop
    while(1)
    {
        // Timing - inputs ============================================================================
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        // Get input
        keyRight = (0x8000 & GetAsyncKeyState((unsigned char)('\x27'))) != 0;
        keyLeft = (0x8000 & GetAsyncKeyState((unsigned char)('\x25'))) != 0;

        if(keyRight && !keyRightOld)
        {
            snakeDirection++;
            if(snakeDirection == 4)
            {
                snakeDirection = 0;
            }
        }

        if(keyLeft && !keyLeftOld)
        {
            snakeDirection--;
            if(snakeDirection == -1)
            {
                snakeDirection = 3;
            }
        }

        keyRightOld = keyRight;
        keyLeftOld = keyLeft;

        // Game logic =================================================================================
        // Update snake position
        switch (snakeDirection)
        {
        case 0: // up
            snake.push_front({snake.front().x, snake.front().y - 1});
            break;
        case 1: // right
            snake.push_front({snake.front().x + 1, snake.front().y});
            break;
        case 2: // down
            snake.push_front({snake.front().x, snake.front().y + 1});
            break;
        case 3: // left
            snake.push_front({snake.front().x - 1, snake.front().y});
            break;
        default:
            break;
        }

        // Chop off snakes tail
        snake.pop_back();

        // Display ====================================================================================
        // Clear screen

        for (int i = 0; i < screenWidth * screenWidth; i++)
        {
            screen[i] = L' ';
        }

        // Draw stats and border
        for(int i = 0; i < screenWidth; i++)
        {
            screen[i] = L'=';
            screen[2 * screenWidth + i] = L'=';
        }
        wsprintf(&screen[screenWidth + 5], L"- - - - - - - - - - - - - - S N A K E - - - - - - - - - - - - - - SCORE: %d", score);

        // Draw snake body
        for(auto s: snake)
        {
            screen[s.y * screenWidth + s.x] = snakeDead ? L'+' : L'O';
        }

        // Draw snake head
        screen[snake.front().y * screenWidth + snake.front().x] = snakeDead ? L'X' : L'@';

        // Draw food
        screen[foodY * screenWidth + foodX] = L'%';

        // Display frame
        WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, {0, 0}, &bytesWritten);
    }

	CloseHandle(console);
	std::cout << "Game Over" << std::endl;
	system("pause");
	return 0;
}