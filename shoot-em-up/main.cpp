#include <algorithm>
#include <array>
#include <cstdint>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

class Shmup : public olc::PixelGameEngine
{
public:
    Shmup()
    {
        sAppName = "Shoot'em up!";
    }

    olc::vf2d playerPos;
    olc::Sprite* sprPlayer = nullptr;
    float playerSpeed = 200.f;

private:
public:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        sprPlayer = new olc::Sprite("img/Emissary.png");

        playerPos = {ScreenWidth() / 2.0f, ScreenHeight() / 2.0f};
        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // INPUT ---------------------------------------------------------------------------------------------------------
        if(GetKey(olc::W).bHeld || GetKey(olc::UP).bHeld) playerPos.y -= playerSpeed * fElapsedTime;
        if(GetKey(olc::S).bHeld || GetKey(olc::DOWN).bHeld) playerPos.y += playerSpeed * fElapsedTime;
        if(GetKey(olc::A).bHeld || GetKey(olc::LEFT).bHeld) playerPos.x -= playerSpeed * fElapsedTime;
        if(GetKey(olc::D).bHeld || GetKey(olc::RIGHT).bHeld) playerPos.x += playerSpeed * fElapsedTime;

        // ---------------------------------------------------------------------------------------------------------------


        // DISPLAY -------------------------------------------------------------------------------------------------------
        Clear(olc::BLACK);

        DrawSprite(playerPos, sprPlayer);

        return true;
    }
};

int main()
{
    Shmup demo;
    // construct screen
    if (demo.Construct(640, 480, 2, 2))
   // if (demo.Construct(2560, 1920, 1, 1))
    {
        demo.Start();
    }
    return 0;
}
