/*
    Breakout

    - Based on https://www.youtube.com/watch?v=Il6kREcjdzE - for learning purposes

    - run: g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
 
*/

#include <iostream>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

class Breakout : public olc::PixelGameEngine
{
public:
    Breakout()
    {
        sAppName = "Breakout";
    }

private:
    std::string level;
    int width = 16;
    int height = 15;
    int block = 8;

    float bat = 64.0;

    float ballX = 64.0;
    float ballY = 64.0;
    float ballDX = 0.0;
    float ballDY = 0.0;

protected:
    virtual bool OnUserCreate() override
    {
        level += "################";
        level += "#..............#";
        level += "#...11111111...#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";
        level += "#..............#";

        float angle = (rand() / (float)(RAND_MAX)) * 3.14159f * 2.0f;
        ballDX = cosf(angle);
        ballDY = sinf(angle);

        return true;
    }

    // called one per frame
    bool OnUserUpdate(float fElapsedTime) override
    {

        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        int batWidth = 10;

        // INPUT ------------------------------------------------------------------------------------------------------------------
        if(GetKey(olc::Key::LEFT).bHeld)
        {
            bat -= 60 * fElapsedTime;
        }
        if(GetKey(olc::Key::RIGHT).bHeld)
        {
            bat += 60 * fElapsedTime;
        }

        // DRAW -------------------------------------------------------------------------------------------------------------------
        // Draw level
        for (int y = 0; y < height; y++)
        {
            for (int x = 0; x < width; x++)
            {
                switch (level[y * width + x])
                {
                    case '#':
                        FillRect(x * block, y * block, block, block, olc::WHITE);
                        break;
                    case '1':
                        FillRect(x * block, y * block,  block, block, olc::GREEN);
                        break;
                    case '.':
                        FillRect(x * block, y * block,  block, block, olc::BLACK);
                        break;
                    default:
                        break;
                }
            }
        }

        // Draw ball
        FillCircle(ballX, ballY, 2.0f, olc::YELLOW);

        // Draw bat
        DrawLine(bat - batWidth, height * block - 2, bat + batWidth, height * block - 2, olc::WHITE);

        return true;
    }
};

int main()
{
    Breakout game;
    if (game.Construct(256, 240, 4, 4))
    {
        game.Start();
    }

    return 0;
}