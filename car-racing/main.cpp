/*
    Retro Arcade Racing Game

    - Based on: https://www.youtube.com/watch?v=KkMZI5Jbf18 - for learning purposes;

    - Based on: https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_RetroArcadeRacer.cpp

    - g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

    - TODO:
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

        /* DrawString(carPos, 80, "   ||####||   ", olc::BLACK); // transparent?
         DrawString(carPos, 81, "      ##      ", olc::BLACK);
         DrawString(carPos, 82, "     ####     ", olc::BLACK);
         DrawString(carPos, 83, "     ####     ", olc::BLACK);
         DrawString(carPos, 84, "|||  ####  |||", olc::BLACK);
         DrawString(carPos, 85, "|||########|||", olc::BLACK);
         DrawString(carPos, 86, "|||  ####  |||", olc::BLACK);*/

        int carDirection = 0;
        switch (carDirection)
		{
		case 0:
			DrawString(carPos, ScreenHeight() - 64, "   ||####||   ", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 56, "      ##      ", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 48, "     ####     ", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 40, "     ####     ", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 32, "|||  ####  |||", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 24, "|||########|||", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 16, "|||  ####  |||", olc::BLACK);
			break;

		case +1:
			DrawString(carPos, ScreenHeight() - 64, "      //####//", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 56, "         ##   ", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 48, "       ####   ", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 40, "      ####    ", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 32, "///  ####//// ", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 24, "//#######///O ", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 16, "/// #### //// ", olc::BLACK);
			break;

		case -1:
			DrawString(carPos, ScreenHeight() - 64, "\\\\####\\\\      "   , olc::BLACK);
			DrawString(carPos, ScreenHeight() - 56, "   ##         "       , olc::BLACK);
			DrawString(carPos, ScreenHeight() - 48, "   ####       "       , olc::BLACK);
			DrawString(carPos, ScreenHeight() - 40, "    ####      "       , olc::BLACK);
			DrawString(carPos, ScreenHeight() - 32, " \\\\\\\\####  \\\\\\", olc::BLACK);
			DrawString(carPos, ScreenHeight() - 24, " O\\\\\\#######\\\\"  , olc::BLACK);
			DrawString(carPos, ScreenHeight() - 16, " \\\\\\\\ #### \\\\\\", olc::BLACK);
			break;
		}

        return true;
    }
};

int main()
{
    CarRacing game;
    //if (game.Construct(160, 100, 8, 8))
    if (game.Construct(640, 300, 2, 2))
    {
        game.Start();
    }

    return 0;
}