/*
    Retro Arcade Racing Game

    - Based on: https://www.youtube.com/watch?v=KkMZI5Jbf18 - for learning purposes;

    - g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

*/

#include <iostream>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

class CarRacing : public olc::PixelGameEngine
{
public:
    CarRacing()
    {
        sAppName = "Retro Arcade Racing Game";
    }

private:
    float carPosX = 0; // -1 0 +1


protected:
    virtual bool OnUserCreate() override
    {
        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        for (int y = 0; y < ScreenHeight() / 2; y++)
        {
            for (int x = 0; x < ScreenWidth(); x++)
            {
                float middlePoint = 0.5;
                float roadWidth = 0.6;
                float clipWidth = roadWidth * 0.15;

                roadWidth *= 0.5;

                int leftGrass = (middlePoint - roadWidth - clipWidth) * ScreenWidth();
                int leftClip = (middlePoint - roadWidth) * ScreenWidth();
                int rightGrass = (middlePoint + roadWidth + clipWidth) * ScreenWidth();
                int rightClip = (middlePoint + roadWidth) * ScreenWidth();

                int row = ScreenHeight() / 2 + y;

                // Draw upeer half of screen - black
                // Draw botton half of screen - green - red - grey - red - green
                if (x >= 0 && x < leftGrass)
                {
                    Draw(x, row, olc::GREEN); // grass
                }
                if (x >= leftGrass && x < leftClip)
                {
                    Draw(x, row, olc::RED); // clip
                }
                if (x >= leftClip && x < rightClip)
                {
                    Draw(x, row, olc::GREY); // road
                }
                if (x >= rightClip && x < rightGrass)
                {
                    Draw(x, row, olc::RED); // clip
                }
                if (x >= rightGrass && x < ScreenWidth())
                {
                    Draw(x, row, olc::GREEN); // grass
                }
            }
        }

        // Draw car
        int carPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * carPosX) / 2.0) - 7; // 7 - width of car sprite

        return true;
    }
};

int main()
{
    CarRacing game;
    if (game.Construct(160, 100, 8, 8))
    {
        game.Start();
    }

    return 0;
}