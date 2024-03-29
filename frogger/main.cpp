/*
    Frogger

    - Based on: https://www.youtube.com/watch?v=QJnZ5QmpXOE - for learning purposes;

    - for some reason the file.spr is not working, have to use file.png

    - easy and fun game to work on!

    - TODO:
        - add more thing to make it a game
        - example FROGGER 1981: https://www.youtube.com/shorts/WbP_oXtiwGA?feature=share 
        - https://www.youtube.com/watch?v=afVR6_gt2RY
 
*/

#include <iostream>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

class Frogger : public olc::PixelGameEngine
{
public:
    Frogger()
    {
        sAppName = "Frogger";
    }

private:
    std::vector<std::pair<float, std::string>> vecLanes =
        {
            {0.0, "wwwhhwwwhhwwwhhwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww"},  // wall
            {-3.0, ",,,jllk,,jllllk,,,,,,,jllk,,,,,jk,,,jlllk,,,,jllllk,,,,jlllk,,,,"}, // log
            {3.0, ",,,,jllk,,,,,jllk,,,,jllk,,,,,,,,,jllk,,,,,jk,,,,,,jllllk,,,,,,,"},  // log
            {2.0, ",,jlk,,,,,jlk,,,,,jk,,,,,jlk,,,jlk,,,,jk,,,,jllk,,,,jk,,,,,,jk,,"},  // log
            {0.0, "pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"},
            {-3.0, "....asdf.......asdf....asdf..........asdf........asdf....asdf..."}, // cars
            {3.0, ".....ty..ty....ty....ty.....ty........ty..ty.ty......ty.......ty"},  // buses
            {-4.0, "..zx.....zx.........zx..zx........zx...zx...zx....zx...zx...zx.."},  // cars
            {2.0, "..ty.....ty.......ty.....ty......ty..ty.ty.......ty....ty......."},  // cars
            {0.0, "pppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppppp"}};

    float timeSinceStart = 0.0;
    int cellSize = 8; // one game unit is 8 character in our screen

    float frogX = 8.0;
    float frogY = 9.0;

    bool *bufDanger = nullptr;

    olc::Sprite *sprBus = nullptr;
    olc::Sprite *sprCar1 = nullptr;
    olc::Sprite *sprCar2 = nullptr;
    olc::Sprite *sprFrog = nullptr;
    olc::Sprite *sprHome = nullptr;
    olc::Sprite *sprLog = nullptr;
    olc::Sprite *sprPavement = nullptr;
    olc::Sprite *sprWall = nullptr;
    olc::Sprite *sprWater = nullptr;

protected:
    virtual bool OnUserCreate() override
    {
        sprBus = new olc::Sprite("sprites/bus.png");
        sprCar1 = new olc::Sprite("sprites/car1.png");
        sprCar2 = new olc::Sprite("sprites/car2.png");
        sprFrog = new olc::Sprite("sprites/frog.png");
        sprHome = new olc::Sprite("sprites/home.png");
        sprLog = new olc::Sprite("sprites/log.png");
        sprPavement = new olc::Sprite("sprites/pavement.png");
        sprWall = new olc::Sprite("sprites/wall.png");
        sprWater = new olc::Sprite("sprites/water.png");

        bufDanger = new bool[ScreenWidth() * ScreenHeight()];
        memset(bufDanger, 0, ScreenWidth() * ScreenHeight() * sizeof(bool));

        return true;
    }

    bool OnUserUpdate(float fElapsedTime) override
    {
        // called one per frame
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLACK);

        timeSinceStart += fElapsedTime;

        // INPUT --------------------------------------------------------------------------
        if (GetKey(olc::Key::UP).bReleased)
        {
            frogY -= 1.0;
        }
        if (GetKey(olc::Key::DOWN).bReleased)
        {
            frogY += 1.0;
        }
        if (GetKey(olc::Key::LEFT).bReleased)
        {
            frogX -= 1.0;
        }
        if (GetKey(olc::Key::RIGHT).bReleased)
        {
            frogX += 1.0;
        }

        // Frog is moved by platforms
        if(frogY <= 3)
        {
            frogX -= fElapsedTime * vecLanes[(int)frogY].first;
        }

        // Collision detection - check four corners of frog against danger buffer
        bool tl = bufDanger[(int)(frogY * cellSize + 1) * ScreenWidth() + (int)(frogX * cellSize + 1)]; // top left
        bool tr = bufDanger[(int)(frogY * cellSize + 1) * ScreenWidth() + (int)((frogX + 1) * cellSize - 1)]; // top right
        bool bl = bufDanger[(int)((frogY + 1) * cellSize - 1) * ScreenWidth() + (int)(frogX * cellSize + 1)]; // bottom left
        bool br = bufDanger[(int)((frogY + 1) * cellSize - 1) * ScreenWidth() + (int)((frogX + 1) * cellSize - 1)]; // bottom right

        if(tl || tr || bl || br)
        {
            // frog has been hit
            frogX = 8.0;
            frogY = 9.0;
        }

        // DRAW ---------------------------------------------------------------------------
        // Draw lanes
        int x = -1;
        int y = 0;
        for (auto lane : vecLanes)
        {
            // Find offset
            int startPos = (int)(timeSinceStart * lane.first) % 64;
            if (startPos < 0)
            {
                startPos = 64 - (abs(startPos) % 64);
            }

            int cellOffset = (int)((float)cellSize * timeSinceStart * lane.first) % cellSize;

            for (int i = 0; i < (ScreenWidth() / cellSize) + 2; i++)
            {
                char graphic = lane.second[(startPos + i) % 64];
                switch (graphic)
                {
                case 'a':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprBus, 0, 0, 8, 8);
                    break; // Bus 1
                case 's':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprBus, 8, 0, 8, 8);
                    break; // Bus 2
                case 'd':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprBus, 16, 0, 8, 8);
                    break; // Bus 3
                case 'f':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprBus, 24, 0, 8, 8);
                    break; // Bus 4

                case 'j':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprLog, 0, 0, 8, 8);
                    break; // Log Start
                case 'l':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprLog, 8, 0, 8, 8);
                    break; // Log Middle
                case 'k':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprLog, 16, 0, 8, 8);
                    break; // Log End

                case 'z':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprCar1, 0, 0, 8, 8);
                    break; // Car1 Back
                case 'x':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprCar1, 8, 0, 8, 8);
                    break; // Car1 Front

                case 't':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprCar2, 0, 0, 8, 8);
                    break; // Car2 Back
                case 'y':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprCar2, 8, 0, 8, 8);
                    break; // Car2 Front

                case 'w':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprWall, 0, 0, 8, 8);
                    break; // Wall
                case 'h':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprHome, 0, 0, 8, 8);
                    break; // Home
                case ',':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprWater, 0, 0, 8, 8);
                    break; // Water
                case 'p':
                    DrawPartialSprite((x + i) * cellSize - cellOffset, y * cellSize, sprPavement, 0, 0, 8, 8);
                    break; // Pavement
                case '.':
                    FillRect((x + i) * cellSize - cellOffset, y * cellSize, cellSize, cellSize, olc::BLACK);
                    break; // Road
                default:
                    break;
                }

                // Fill danger buffer
                for (int j = (x + i) * cellSize - cellOffset; j < (x + i + 1) * cellSize - cellOffset; j++)
                {
                    for (int k = y * cellSize; k < (y + 1) * cellSize; k++)
                    {
                        // check boundaries
                        if (j >= 0 && j < ScreenWidth() && k >= 0 && k < ScreenHeight())
                        {
                            bufDanger[k * ScreenWidth() + j] = !(graphic == '.' || graphic == 'j' || graphic == 'k' || graphic == 'l' || graphic == 'p' || graphic == 'h');
                        }
                    }
                }
            }
            y++;
        }

        // Draw frog - mask
        SetPixelMode(olc::Pixel::MASK);
        DrawSprite(frogX * cellSize, frogY * cellSize, sprFrog);
        SetPixelMode(olc::Pixel::NORMAL);

        return true;
    }
};

int main()
{
    Frogger game;
    if (game.Construct(128, 80, 12, 12))
    {
        game.Start();
    }

    return 0;
}