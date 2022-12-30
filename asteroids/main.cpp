#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

/*
v = p2 - p1 / t
a = v2 - v1 / t
p2 = vt - p1
v2 = at + v1
p = {x, y}
v, a ~ {x, y}
*/

class Asteroids : public olc::PixelGameEngine
{
public:
    Asteroids()
    {
        sAppName = "Asteroids";
    }
private:
    struct spaceObject
    {
        float x;  // position
        float y;
        float dx; // speed direction
        float dy;
        int size; // of asteroids
    };

    std::vector<spaceObject> vecAsteroids; // collect the different objects in vectors
    

public:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        vecAsteroids.push_back({20.0f, 20.0f, 8.0f, -6.0f, (int)16});
        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // clear console
        PixelGameEngine::ConsoleClear();
        // set the background color
        Clear(olc::BLACK);

        // update and draw asteroids
        for (auto &a : vecAsteroids)
        {
            a.x += a.dx * fElapsedTime; // fElapsedTime - time between frames
            a.y += a.dy * fElapsedTime;
            wrapCoordinates(a.x, a.y, a.x, a.y);
            
            for (int x = 0; x < a.size; x++)
            {
                for (int y = 0; y < a.size; y++)
                {
                    Draw(a.x + x, a.y + y, olc::RED);
                }
            }
        }
    
        return true;
    }

    // toroidal mapping - so the asteroids will appear on the other side of screen
    void wrapCoordinates(float ix, float iy, float &ox, float &oy)
    {
        ox = ix;
        oy = iy;

        if(ix < 0.0f)
        {
            ox = ix + (float)ScreenWidth();
        }
        if(ix >= (float)ScreenWidth())
        {
            ox = ix - (float)ScreenWidth();
        }
        if(iy < 0.0f)
        {
            oy = iy + (float)ScreenHeight();
        }
        if (iy >= (float)ScreenHeight())
        {
            oy = iy - (float)ScreenHeight();
        }
    }

    virtual bool Draw(int32_t x, int32_t y, olc::Pixel p = olc::WHITE) // override;
    {
        float fx, fy;
        wrapCoordinates(x, y, fx, fy);
        if (PixelGameEngine::Draw(fx, fy, p))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
};

int main()
{
    Asteroids demo;
    // construct screen
    // 160 wide 180 tall - 8 pixel per character
    if (demo.Construct(160, 180, 8, 8))
    {
        demo.Start();
    }
    return 0;
}