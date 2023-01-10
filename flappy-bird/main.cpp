#include <iostream>
#include <string>
#include <list>

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

    float sectionWidth;
    std::list<int> listSection;
    float levelPosition;

protected:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        listSection = {0, 0, 0, 0};
        sectionWidth = (float)ScreenWidth() / (float)(listSection.size() - 1);
        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        if (GetKey(olc::Key::SPACE).bPressed && birdVelocity >= gravity / 10.0f)
        {
            birdAcceleration = 0.0f;
            birdVelocity = - gravity / 4.0f;
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

        levelPosition += 14.0f * fElapsedTime;

        if(levelPosition > sectionWidth)
        {
            levelPosition -= sectionWidth;
            listSection.pop_front();
            int i = rand() % (ScreenHeight() - 20);
            if (i <= 10) i = 0;
            listSection.push_back(i);
        }

        Clear(olc::BLACK); // clear screen

        int section = 0;
        for(auto s : listSection)
        {
            if (s != 0)
            {
                // FillRect(int32_t x, int32_t y, int32_t w, int32_t h, Pixel p)
                // Fill(section * sectionWidth + 10 - levelPosition, ScreenHeight() - s, section * sectionWidth + 15 - levelPosition, ScreenHeight(), PIXEL_SOLID, FG_GREEN);
                FillRect(section * sectionWidth + 10 - levelPosition, ScreenHeight() - s, section * sectionWidth + 15 - levelPosition, ScreenHeight(), olc::GREEN);
                //FillRect(section * sectionWidth + 10 - levelPosition, section * sectionWidth + 15 - levelPosition, ScreenHeight() - s, ScreenHeight(), olc::GREEN);
               // FillRect(ScreenWidth(), section * sectionWidth + 15 - levelPosition, 5, ScreenHeight() - 10, olc::GREEN);

               FillRect(section * sectionWidth + 10 - levelPosition, 0, section * sectionWidth + 15 - levelPosition, ScreenHeight() - s - 15, olc::GREEN);
            }
            section++;
        }
        // 
        int birdX = (int)(ScreenWidth() / 3.0f);

        // Draw bird
        if (birdVelocity > 0) // bird falling
        {
            Draw(birdX, birdPosition + 0, olc::YELLOW);
            //DrawString(birdX, birdPosition + 0, "\\\\\\", olc::WHITE, 1);
            //DrawString(birdX, birdPosition + 1, "<\\\\\\=Q", olc::WHITE, 1);
        }
        else // moving up
        {
            Draw(birdX, birdPosition + 0, olc::VERY_DARK_YELLOW);
            //DrawString(birdX, birdPosition + 0, "<///=Q>", olc::WHITE, 1);
            //DrawString(birdX, birdPosition + 1, "///", olc::WHITE, 1);
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

