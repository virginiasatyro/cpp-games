#include <algorithm>
#include <array>
#include <cstdint>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

class Snake : public olc::PixelGameEngine
{
public:
    Snake()
    {
        sAppName = "Snake";
    }

    olc::vf2d headPosition;
    float speed = 40.0f;

    //std::list<SnakeSegment> listSnakeSeg;

private: 
    

public:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        // SnakeSegment snake;
        headPosition.x = ScreenWidth() / 2.0f;
        headPosition.y = ScreenHeight() / 2.0f;

        // listSnakeSeg.push_back(snake);

        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // INPUT ---------------------------------------------------------------------------------------------------------
        if (GetKey(olc::W).bHeld || GetKey(olc::UP).bHeld)
        {
            headPosition.y -= speed  * fElapsedTime;
        }
        if (GetKey(olc::S).bHeld || GetKey(olc::DOWN).bHeld)
        {
            headPosition.y += speed * fElapsedTime;
        }
        if (GetKey(olc::A).bHeld || GetKey(olc::LEFT).bHeld)
        {
            headPosition.x -= speed * fElapsedTime;
        }
        if (GetKey(olc::D).bHeld || GetKey(olc::RIGHT).bHeld)
        {
            headPosition.x += speed * fElapsedTime;
        }

        // Spaceship cant go off screen
        if (headPosition.x <= 0)
        {
            headPosition.x = 0;
        }
        if (headPosition.y <= 0)
        {
            headPosition.y = 0;
        }
        if (headPosition.x + 1.0f >= (float)ScreenWidth())
        {
            headPosition.x = (float)ScreenWidth() - 1.0f; // pixel size
        }
        if (headPosition.y + 1.0f >= (float)ScreenHeight())
        {
            headPosition.y = (float)ScreenHeight() - 1.0f;
        }

        // ---------------------------------------------------------------------------------------------------------------

        // DRAW ----------------------------------------------------------------------------------------------------------
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLUE);

       // for (auto& snakeSeg : listSnakeSeg)
      //  {
            Draw(headPosition.x, headPosition.y, olc::WHITE);
       // }

        return true;
    }
};

int main()
{
    Snake demo;
    // Construct screen
    // if (demo.Construct(640, 480, 2, 2))
    if (demo.Construct(64, 64, 16, 16))
    {
        demo.Start();
    }
    return 0;
}
