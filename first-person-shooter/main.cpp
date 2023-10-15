/*
    First Person Shooter

    - Based on https://www.youtube.com/watch?v=xW8skO7MFYw - for learning purposes;
    - Compiling: g++ -o main.exe main.cpp
    - UNICODE: The Unicode Standard is the specification of an encoding scheme for written
      characters and text. It is a universal standard that enables consistent encoding of 
      multilingual text and allows text data to be interchanged internationally without conflict.
      https://www.ibm.com/docs/en/i/7.4?topic=set-unicode-standard-c-only
    - Based on https://pt.wikipedia.org/wiki/Wolfenstein_3D
    - Wolfenstein 3D: vide renderização ray casting gif;
    - Para rodar corretamente é necessário ajustar manualmente o tamanho da janela e definir 
      os valores de largura e altura nas propriedades do console (prompt);
    - Como o computador compartilha recursos com outras aplicações, não é possível saber com que
      frequência a tela será atualizada. Por isso, é necessário utiliza o tempo para computar os 
      frames e ter certeza que tudo será calculado seguindo um padrão;
*/

#ifndef UNICODE // https://stackoverflow.com/questions/13977388/error-cannot-convert-const-wchar-t-13-to-lpcstr-aka-const-char-in-assi
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <iostream>
#include <windows.h>
#include <math.h>
#include <chrono>

#ifdef UNICODE_WAS_UNDEFINED
#undef UNICODE
#endif

constexpr int screenWidth = 120;
constexpr int screenHeight = 40;
constexpr int mapWidth = 16;
constexpr int mapHeight = 16;
constexpr float PI = 3.14159;

float playerX = 8.0;
float playerY = 8.0;
float playerA = 0.0;

float FOV = PI / 4.0; // field of view - angulo de visão
float depth = 16.0;

int main()
{
    // std::cout << "Hello" << std::endl;

    // Create screen buffer
    wchar_t *screen = new wchar_t[screenWidth * screenHeight];
    HANDLE console = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(console);
    DWORD bytesWritten = 0;

    std::wstring map;

    map += L"################";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    auto time1 = std::chrono::system_clock::now();
    auto time2 = std::chrono::system_clock::now();

    // Game loop
    while (1)
    {
        // Time ---------------------------------------------------------------------------------------
        time2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = time2 - time1;
        time1 = time2;
        float fElapsedTime = elapsedTime.count();

        // Controls 
        // Hanle CCW Rotation -------------------------------------------------------------------------
        if(GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            playerA -= (0.1) * fElapsedTime;
        }
        if(GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            playerA += (0.1) * fElapsedTime;
        }

        // --------------------------------------------------------------------------------------------
        for (int x = 0; x < screenWidth; x++)
        {
            // for each colunm, calculate the projected ray angle into world space
            float rayAngle = (playerA - FOV / 2.0) + ((float)x / (float)screenWidth) * FOV;

            float distanceToWall = 0;
            bool hitWall = false;

            float eyeX = sinf(rayAngle); // unit vector for ray in player space
            float eyeY = cosf(rayAngle);

            while (!hitWall && (distanceToWall < depth))
            {
                distanceToWall += 0.1f;

                int testX = (int)(playerX + eyeX * distanceToWall);
                int testY = (int)(playerY + eyeY * distanceToWall);

                // Test if ray is out of bounds
                if ((testX < 0) || (testX >= mapWidth) || (testY < 0) || (testY >= mapHeight))
                {
                    hitWall = true;
                    distanceToWall = depth; // just set to maximum depth
                }
                else
                {
                    // ray is inbounds so test to see if the ray is a wall block
                    if(map[testY * mapWidth + testX] == '#')
                    {
                        hitWall = true;
                    }
                }
            }

            // Calculate distance to ceiling and floor
            int ceiling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceToWall);
            int floor = screenHeight - ceiling;

            for (int y = 0; y < screenHeight; y++)
            {
                if(y < ceiling)
                {
                    screen[y * screenWidth + x] = ' '; // ceiling
                }
                else if ((y > ceiling) && (y <= floor))
                {
                    screen[y * screenWidth + x] = '#'; // wall
                }
                else
                {
                    screen[y * screenWidth + x] = ' '; // floor
                }
            }
        }

        screen[screenWidth * screenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, {0, 0}, &bytesWritten);
    }

    return 0;
}