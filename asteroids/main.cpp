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
        float angle;
    };

    std::vector<spaceObject> vecAsteroids; // collect the different objects in vectors
    spaceObject spaceship;

public:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        vecAsteroids.push_back({20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f});

        // initialize spaceship position
        spaceship.x = ScreenWidth() / 2.0f;
        spaceship.y = ScreenHeight() / 2.0f;
        spaceship.dx = 0.0f;
        spaceship.dy = 0.0f;
        spaceship.angle = 0.0f;
        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // clear console
        PixelGameEngine::ConsoleClear();
        // set the background color
        Clear(olc::BLACK);

        // steer
        if(GetKey(olc::Key::LEFT).bHeld)
        {
            spaceship.angle -= 5.0f * fElapsedTime;
        }
        if(GetKey(olc::Key::RIGHT).bHeld)
        {
            spaceship.angle += 5.0f * fElapsedTime;
        }

        // thrust
        if(GetKey(olc::Key::UP).bHeld)
        {
            // acceleration changes velocity (with respect to time)
            spaceship.dx += sin(spaceship.angle) * 20.0f * fElapsedTime;
            spaceship.dy += -cos(spaceship.angle) * 20.0f * fElapsedTime;
        }

        // velocity changes position (with respect to time)
        spaceship.x += spaceship.dx * fElapsedTime;
        spaceship.y += spaceship.dy * fElapsedTime;

        wrapCoordinates(spaceship.x, spaceship.y, spaceship.x, spaceship.y);

        // draw spaceship
        float mx[3] = {0.0f, -2.5f, +2.5f}; // spaceship model vertices
        float my[3] = {-5.5f, +2.5f, +2.5f};

        float sx[3], sy[3];

        // rotate
        for (int i = 0; i < 3; i++)
        {
            sx[i] = mx[i] * cosf(spaceship.angle) - my[i] * sinf(spaceship.angle);
            sy[i] = mx[i] * sinf(spaceship.angle) + my[i] * cosf(spaceship.angle);
        }

        // translate
        for(int i = 0; i < 3; i++)
        {
            sx[i] = sx[i] + spaceship.x;
            sy[i] = sy[i] + spaceship.y;
        }

        // draw closed polygon
        for (int i = 0; i < 4; i++)
        {
            int j = i + 1;
            DrawLine(sx[i % 3], sy[i % 3], sx[j % 3], sy[j % 3]); // module - to make sure all 3 lines are draw
        }

        // update and draw asteroids
        for (auto &asteroid : vecAsteroids)
        {
            asteroid.x += asteroid.dx * fElapsedTime; // fElapsedTime - time between frames
            asteroid.y += asteroid.dy * fElapsedTime;
            wrapCoordinates(asteroid.x, asteroid.y, asteroid.x, asteroid.y);
            
            for (int x = 0; x < asteroid.size; x++)
            {
                for (int y = 0; y < asteroid.size; y++)
                {
                    Draw(asteroid.x + x, asteroid.y + y, olc::RED);
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