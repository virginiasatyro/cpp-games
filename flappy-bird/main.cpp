#include <iostream>
#include <string>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

class FlappyBird : public olc::PixelGameEngine
{
public:
    FlappyBird()
    {
        sAppName = "Flappy Bird";
    }

private:
    float birdPosition = 0.0f;
    float birdVelocity = 0.0f;
    float birdAcceleration = 0.0f;

    float gravity = 100.0f;

protected:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        if (GetKey(olc::Key::SPACE).bPressed)
        {
            birdAcceleration = 0.0f;
            birdVelocity = -gravity / 4.0f;
        }
        else
        {
            birdAcceleration += gravity * fElapsedTime;
        }

        if (birdAcceleration >= gravity) // add some control
        {
            birdAcceleration = gravity;
        }

        birdVelocity += birdAcceleration * fElapsedTime;
        birdPosition += birdVelocity * fElapsedTime;

        Clear(olc::BLACK); // clear screen
        
        int birdX = (int)(ScreenWidth() / 3.0f);

        // Draw bird
        if (birdVelocity > 0) // bird falling
        {
            DrawString(birdX, birdPosition + 0, "\\\\\\", 1);
            DrawString(birdX, birdPosition + 1, "<\\\\\\=Q", 1);
        }
        else // moving up
        {
            DrawString(birdX, birdPosition + 0, "<///=Q>");
            DrawString(birdX, birdPosition + 1, "///");
        }

        return true;
    }
};

int main()
{
    FlappyBird game;

    if (game.Construct(80, 48, 16, 16))
    {
        game.Start();
    }
    return 0;
}

