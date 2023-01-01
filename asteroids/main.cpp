#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

constexpr float piX2 = 6.283185f;
constexpr float invalidCoordinate = -100.0f;

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
        float x; // position
        float y;
        float dx; // speed direction
        float dy;
        int size; // of asteroids
        float angle;
    };

    std::vector<spaceObject> vecAsteroids; // collect the different objects in vectors
    std::vector<spaceObject> vecBullets;
    spaceObject spaceship;
    int score = 0;
    bool spaceshipDead = false;

    std::vector<std::pair<float, float>> vecModelShip;
    std::vector<std::pair<float, float>> vecModelAsteroid;

public:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        vecModelShip =
        {
            {0.0f, -5.0f},
            {-2.5f, +2.5f},
            {+2.5f, +2.5f}
        }; // simple isoceles triangle

        int verts = 20; // each asteroid have 20 points
        for(int i = 0; i < verts; i++)
        {
            float radius = 1.0f;
            float a = ((float)i / (float)verts) * piX2; // 2 * pi
            vecModelAsteroid.push_back(std::make_pair(radius * sinf(a), radius * cosf(a)));
        }

        resetGame();

        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // if spaceship is dead, reset the game
        if(spaceshipDead)
        {
            resetGame();
        }

        // clear console
        PixelGameEngine::ConsoleClear();
        // set the background color
        Clear(olc::BLACK);

        // steer
        if (GetKey(olc::Key::LEFT).bHeld)
        {
            spaceship.angle -= 5.0f * fElapsedTime;
        }
        if (GetKey(olc::Key::RIGHT).bHeld)
        {
            spaceship.angle += 5.0f * fElapsedTime;
        }

        // thrust
        if (GetKey(olc::Key::UP).bHeld)
        {
            // acceleration changes velocity (with respect to time)
            spaceship.dx += sin(spaceship.angle) * 20.0f * fElapsedTime;
            spaceship.dy += -cos(spaceship.angle) * 20.0f * fElapsedTime;
        }

        // velocity changes position (with respect to time)
        spaceship.x += spaceship.dx * fElapsedTime;
        spaceship.y += spaceship.dy * fElapsedTime;

        // keep spaceship in gamespace
        wrapCoordinates(spaceship.x, spaceship.y, spaceship.x, spaceship.y);

        // check ship collision with asteroids
        for (auto &asteroid : vecAsteroids)
        {
            if (isPointInsideCircle(asteroid.x, asteroid.y, asteroid.size, spaceship.x, spaceship.y))
            {
                spaceshipDead = true;
            }
        }

        // fire bullet
        if(GetKey(olc::Key::SPACE).bReleased)
        {
            vecBullets.push_back({spaceship.x, spaceship.y, 50.0f * sinf(spaceship.angle), -50.0f * cosf(spaceship.angle), 0, 0.0f});
        }

        // update and draw asteroids
        for (auto &asteroid : vecAsteroids)
        {
            asteroid.x += asteroid.dx * fElapsedTime; // fElapsedTime - time between frames
            asteroid.y += asteroid.dy * fElapsedTime;
            wrapCoordinates(asteroid.x, asteroid.y, asteroid.x, asteroid.y);

            DrawWireFrameModel(vecModelAsteroid, asteroid.x, asteroid.y, asteroid.angle, asteroid.size, olc::YELLOW);
        }

        std::vector<spaceObject> newAsteroids;

        // update and draw bullets
        for (auto &bullet : vecBullets)
        {
            bullet.x += bullet.dx * fElapsedTime; // fElapsedTime - time between frames
            bullet.y += bullet.dy * fElapsedTime;
            wrapCoordinates(bullet.x, bullet.y, bullet.x, bullet.y);

            Draw(bullet.x, bullet.y);

            // check collision with asteroids
            for (auto &asteroid : vecAsteroids)
            {
                if(isPointInsideCircle(asteroid.x, asteroid.y, asteroid.size, bullet.x, bullet.y))
                {
                    // asteroid hit
                    bullet.x = invalidCoordinate; // force bullet to be out of screen

                    if(asteroid.size > 4)
                    {
                        // create two child asteroids
                        float angle1 = ((float)rand() / (float)RAND_MAX) * piX2; // randon angle betwwen 0 and 2pi
                        float angle2 = ((float)rand() / (float)RAND_MAX) * piX2; // randon angle betwwen 0 and 2pi
                        newAsteroids.push_back({asteroid.x, asteroid.y, 10.0f * sinf(angle1), 10.0f * cosf(angle1), (int)asteroid.size >> 1, 0.0f });
                        newAsteroids.push_back({asteroid.x, asteroid.y, 10.0f * sinf(angle2), 10.0f * cosf(angle2), (int)asteroid.size >> 1, 0.0f });
                    }
                    // kill the old asteroid
                    asteroid.x = invalidCoordinate;
                }
            }
        }

        // append new asteroids to existing vector
        for (auto asteroid : newAsteroids)
        {
            vecAsteroids.push_back(asteroid);
        }

        // remove off screen bullets
        if(vecBullets.size() > 0)
        {
            // [&] - lambda function - return a interator, sorts vec of bullets, if fails the if criteria,
            // it is at the end, return true if the bullets are off the screen
            auto i = remove_if(vecBullets.begin(), vecBullets.end(), [&](spaceObject o)
                               { return (o.x < 1 || o.y < 1 || o.x >= ScreenWidth() || o.x >= ScreenHeight()); });
            if (i != vecBullets.end())
            {
                vecBullets.erase(i);
            }
        }

        // remove off screen asteroids
        if(vecAsteroids.size() > 0)
        {
            auto i = remove_if(vecAsteroids.begin(), vecAsteroids.end(), [&](spaceObject o)
                               { return (o.x < 0); });
            if (i != vecAsteroids.end())
            {
                vecAsteroids.erase(i);
            }
        }

        // draw spaceship
        DrawWireFrameModel(vecModelShip, spaceship.x, spaceship.y, spaceship.angle);

        return true;
    }

    // toroidal mapping - so the asteroids will appear on the other side of screen
    void wrapCoordinates(float ix, float iy, float &ox, float &oy)
    {
        ox = ix;
        oy = iy;

        if (ix < 0.0f)
        {
            ox = ix + (float)ScreenWidth();
        }
        if (ix >= (float)ScreenWidth())
        {
            ox = ix - (float)ScreenWidth();
        }
        if (iy < 0.0f)
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

    void DrawWireFrameModel(const std::vector<std::pair<float, float>> &vecModelCoordinates, float x, float y, float r = 0.0f, float s = 1.0f, olc::Pixel col = olc::WHITE)
    {
        // pair.first = x coordinate
        // pair.second = y coordinate

        // create translated model vector of coordinate pairs
        std::vector<std::pair<float, float>> vecTransformedCoordinates;
        int verts = vecModelCoordinates.size();
        vecTransformedCoordinates.resize(verts);

        // rotate
        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r);
            vecTransformedCoordinates[i].second = vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r);
        }

        // scale
        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
            vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
        }

        // translate
        for (int i = 0; i < verts; i++)
        {
            vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
            vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
        }

        // draw closed polygon
        for (int i = 0; i < verts + 1; i++)
        {
            int j = (i + 1);
            DrawLine((int)vecTransformedCoordinates[i % verts].first, (int)vecTransformedCoordinates[i % verts].second,
                     (int)vecTransformedCoordinates[j % verts].first, (int)vecTransformedCoordinates[j % verts].second, col, 0xFFFFFFFF);
        }
    }

    bool isPointInsideCircle(float cx, float cy, float radius, float x, float y)
    {
        return sqrt((x - cx)*(x - cx) + (y - cy) * (y - cy)) < radius;
    }

    void resetGame()
    {
        vecAsteroids.clear();
        vecBullets.clear();

        vecAsteroids.push_back({20.0f, 20.0f, 8.0f, -6.0f, (int)16, 0.0f});
        vecAsteroids.push_back({100.0f, 20.0f, -5.0f, 3.0f, (int)16, 0.0f});

        // initialize spaceship position
        spaceship.x = ScreenWidth() / 2.0f;
        spaceship.y = ScreenHeight() / 2.0f;
        spaceship.dx = 0.0f;
        spaceship.dy = 0.0f;
        spaceship.angle = 0.0f;

        spaceshipDead = false;
        score = 0;
    }
};

int main()
{
    Asteroids demo;
    // construct screen
    // 160 wide 180 tall - 8 pixel per character (changed to 4)
    if (demo.Construct(160, 180, 4, 4))
    {
        demo.Start();
    }
    return 0;
}

/*
v = p2 - p1 / t
a = v2 - v1 / t
p2 = vt - p1
v2 = at + v1
p = {x, y}
v, a ~ {x, y}
*/
