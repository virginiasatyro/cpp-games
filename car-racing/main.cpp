/*
    Retro Arcade Racing Game

    - Based on: https://www.youtube.com/watch?v=KkMZI5Jbf18 - for learning purposes;

    - Based on: https://github.com/OneLoneCoder/Javidx9/blob/master/ConsoleGameEngine/SmallerProjects/OneLoneCoder_RetroArcadeRacer.cpp

    - g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

    - A very interesting way to create the ilusion of 3D perpective is to use periodic oscilators;

    - TODO:
        - study more sin functions for periodic oscilators;
        - enhance the way we track the section?
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
    float carDistance = 0;
    float carSpeed = 0.0;

    float curvature = 0.0;

    std::vector<std::pair<float, float>> vecTrack; // curvature, distance

protected:
    virtual bool OnUserCreate() override
    {
        // Creating a track
        // add a short section to start track
        vecTrack.push_back(std::make_pair(0.0, 10.0));
        vecTrack.push_back(std::make_pair(0.0, 200.0));
        vecTrack.push_back(std::make_pair(1.0, 200.0));
        vecTrack.push_back(std::make_pair(0.0, 400.0));
        vecTrack.push_back(std::make_pair(-1.0, 100.0));
        vecTrack.push_back(std::make_pair(0.0, 200.0));
        vecTrack.push_back(std::make_pair(-1.0, 200.0));
        vecTrack.push_back(std::make_pair(1.0, 200.0));
        vecTrack.push_back(std::make_pair(0.0, 400.0));

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // INPUT --------------------------------------------------------------------------
        if (GetKey(olc::Key::UP).bHeld)
        {  
            carSpeed += 2.0 * fElapsedTime;
        }
        else
        {
            carSpeed -= 1.0 * fElapsedTime;
        }

        // clamp speed
        if (carSpeed < 0.0)
        {
            carSpeed = 0.0;
        }
        if (carSpeed > 1.0)
        {
            carSpeed = 1.0;
        }

        // Move car along track according to speed
        carDistance += (70.0 * carSpeed) * fElapsedTime;

        // --------------------------------------------------------------------------
        // get point on track
        float offset = 0;
        int trackSection = 0;

        // Find position on track
        while(trackSection < vecTrack.size() && offset <= carDistance)
        {
            offset += vecTrack[trackSection].second;
            trackSection++;
        }

        float targetCurvature = vecTrack[trackSection - 1].first;

        float trackCurveDiff = (targetCurvature - curvature) * fElapsedTime * carSpeed;
        curvature += trackCurveDiff;

        // DRAW - LOGIC -------------------------------------------------------------------
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::DARK_BLUE);

        for (int y = 0; y < ScreenHeight() / 2; y++)
        {
            for (int x = 0; x < ScreenWidth(); x++)
            {
                float perspective = (float)y / (ScreenHeight() / 2.0);

                float middlePoint = 0.5 + curvature * powf((1.0 - perspective), 3);
                float roadWidth = 0.1 + perspective * 0.8;
                float clipWidth = roadWidth * 0.15;

                roadWidth *= 0.5;

                int leftGrass = (middlePoint - roadWidth - clipWidth) * ScreenWidth();
                int leftClip = (middlePoint - roadWidth) * ScreenWidth();
                int rightGrass = (middlePoint + roadWidth + clipWidth) * ScreenWidth();
                int rightClip = (middlePoint + roadWidth) * ScreenWidth();

                int row = ScreenHeight() / 2 + y;

                auto grassColour = sinf(20.0 * powf(1.0 - perspective, 3) + carDistance * 0.1f) > 0.0 ? olc::GREEN : olc::DARK_GREEN;
                auto clipColour = sinf(80 * powf(1.0 - perspective, 2) + carDistance) > 0.0 ? olc::RED : olc::WHITE;

                // Draw upeer half of screen - black
                // Draw botton half of screen - green - red - grey - red - green
                if (x >= 0 && x < leftGrass)
                {
                    Draw(x, row, grassColour); // grass
                }
                if (x >= leftGrass && x < leftClip)
                {
                    Draw(x, row, clipColour); // clip
                }
                if (x >= leftClip && x < rightClip)
                {
                    Draw(x, row, olc::GREY); // road
                }
                if (x >= rightClip && x < rightGrass)
                {
                    Draw(x, row, clipColour); // clip
                }
                if (x >= rightGrass && x < ScreenWidth())
                {
                    Draw(x, row, grassColour); // grass
                }
            }
        }

        // Draw car
        int carPos = ScreenWidth() / 2 + ((int)(ScreenWidth() * carPosX) / 2.0) - 7; // 7 - width of car sprite

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