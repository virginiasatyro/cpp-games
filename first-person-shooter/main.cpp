/*
    First Person Shooter

    - Based on https://www.youtube.com/watch?v=xW8skO7MFYw and https://github.com/OneLoneCoder/CommandLineFPS 
    for learning purposes;

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

    - Unicode - https://www.ssec.wisc.edu/~tomw/java/unicode.html

    - Várias lógicas matemática ainda precisa ser mais estudadas para melhor entender parted do
      algoritmo. Pincipalmente em relação aos ângulos.
*/

#ifndef UNICODE // https://stackoverflow.com/questions/13977388/error-cannot-convert-const-wchar-t-13-to-lpcstr-aka-const-char-in-assi
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif

#include <iostream>
#include <windows.h>
#include <math.h>
#include <chrono>
#include <vector>
#include <algorithm>

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
    map += L"#..........#...#";
    map += L"#..........#...#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.......########";
    map += L"#..............#";
    map += L"################";

    auto time1 = std::chrono::system_clock::now();
    auto time2 = std::chrono::system_clock::now();

    // Game loop --------------------------------------------------------------------------------------
    while (1)
    {
        // Time ---------------------------------------------------------------------------------------
        time2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = time2 - time1;
        time1 = time2;
        float fElapsedTime = elapsedTime.count();

        // Controls
        // Hanle CCW Rotation -------------------------------------------------------------------------
        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
        {
            playerA -= (0.8) * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
        {
            playerA += (0.8) * fElapsedTime;
        }
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
        {
            playerX += sinf(playerA) * 5.0 * fElapsedTime;
            playerY += cosf(playerA) * 5.0 * fElapsedTime;

            // Collision detection
            // TODO: need to check if this logic is ok!
            if(map[(int)playerY * mapWidth + (int)playerX] == '#')
            {
                // if player hits a wall, undo
                playerX -= sinf(playerA) * 5.0 * fElapsedTime;
                playerY -= sinf(playerA) * 5.0 * fElapsedTime;
            }
        }
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
        {
            playerX -= sinf(playerA) * 5.0 * fElapsedTime;
            playerY -= cosf(playerA) * 5.0 * fElapsedTime;
            
            // Collision detection
            if(map[(int)playerY * mapWidth + (int)playerX] == '#')
            {
                // if player hits a wall, undo
                playerX += sinf(playerA) * 5.0 * fElapsedTime;
                playerY += sinf(playerA) * 5.0 * fElapsedTime;
            }
        }

        // --------------------------------------------------------------------------------------------
        for (int x = 0; x < screenWidth; x++)
        {
            // for each colunm, calculate the projected ray angle into world space
            float rayAngle = (playerA - FOV / 2.0) + ((float)x / (float)screenWidth) * FOV;

            float distanceToWall = 0;
            bool hitWall = false;
            bool boundary = false;

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
                    if (map[testY * mapWidth + testX] == '#')
                    {
                        hitWall = true;
                        std::vector<std::pair<float, float>> p;

                        for (int tx = 0; tx < 2; tx++)
                        {
                            for (int ty = 0; ty < 2; ty++)
                            {
                                float vx = (float)testX + tx - playerX;
                                float vy = (float)testY + ty - playerY;
                                float d = sqrt(vx * vx + vy * vy); // how far away corner is from player
                                float dot = (eyeX * vx / d) + (eyeY * vy / d);
                                p.push_back(std::make_pair(d, dot));
                            }
                        }

                        // Sort pairs from closest to farthest - lambda function
                        sort(p.begin(), p.end(), [](const std::pair<float, float> &left, const std::pair<float, float> &right)
                             { return left.first < right.first; });

                        float bound = 0.01;
                        // need to study the math/logic more
                        if (acos(p.at(0).second) < bound)
                        {
                            boundary = true;
                        }
                        if (acos(p.at(1).second) < bound)
                        {
                            boundary = true;
                        }
                    }
                }
            }

            // Calculate distance to ceiling and floor
            int ceiling = (float)(screenHeight / 2.0) - screenHeight / ((float)distanceToWall);
            int floor = screenHeight - ceiling;

            short wallShade = ' ';

            if (distanceToWall <= (depth / 4.0))
            {
                wallShade = 0x2588; // ▓ very close
            }
            else if (distanceToWall < (depth / 3.0))
            {
                wallShade = 0x2593; // ▓
            }
            else if (distanceToWall < (depth / 2.0))
            {
                wallShade = 0x2592; // ▒
            }
            else if (distanceToWall < depth)
            {
                wallShade = 0x2591; // ░
            }
            else
            {
                wallShade = ' '; // too far away
            }

            if(boundary)
            {
                wallShade = ' ';
            }

            for (int y = 0; y < screenHeight; y++)
            {
                if (y < ceiling)
                {
                    screen[y * screenWidth + x] = ' '; // ceiling
                }
                else if ((y > ceiling) && (y <= floor))
                {
                    screen[y * screenWidth + x] = wallShade; // wall
                }
                else
                {
                    // shade floor based on distance
                    float b = 1.0 - (((float)y - screenHeight / 2.0) / ((float)screenHeight / 2.0));
                    short floorShade = ' ';
                    if(b < 0.25)      floorShade = '#';
                    else if(b < 0.5)  floorShade = 'x';
                    else if(b < 0.75) floorShade = '.';
                    else if (b < 0.9) floorShade = '-';
                    else              floorShade = ' ';
                    screen[y * screenWidth + x] = floorShade; // floor
                }
            }
        }

        // Display stats
        swprintf_s(screen, 40, L"X=%3.2f, Y=%3.2f, A=%3.2f FPS=%3.2f ", playerX, playerY, playerA, 1.0 / fElapsedTime);

        // Display map
        for(int nx = 0; nx < mapWidth; nx++)
        {
            for (int ny = 0; ny < mapWidth; ny++)
            {
               screen[(ny + 1) * screenWidth + nx] = map[ny * mapWidth + (mapWidth - nx - 1)];
            }
        }

        // Display player
        // careful! no collision detection 
        screen[((int)playerY + 1) * screenWidth + (int)(mapWidth - playerX)] = 'P';

        screen[screenWidth * screenHeight - 1] = '\0';
        WriteConsoleOutputCharacter(console, screen, screenWidth * screenHeight, {0, 0}, &bytesWritten);
    }

    return 0;
}