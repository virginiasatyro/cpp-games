
#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

#include <vector>

/*
v = p2 - p1 / t
a = v2 - v1 / t
p2 = vt - p1
v2 = at + v1
p = {x, y}
v, a ~ {x, y}
*/

class Example : public olc::PixelGameEngine
{
public:
    Example()
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
    // Called once at the start, so create things here
    bool OnUserCreate() override
    {
        vecAsteroids.push_back({20.0f, 20.0f, 8.0f, -6.0f, static_cast<int>(16)});

        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // clear console
        PixelGameEngine::ConsoleClear();

        // update and draw asteroids
        for (auto &asteroid : vecAsteroids)
        {
            asteroid.x += asteroid.dx * fElapsedTime; // fElapsedTime - time between frames
            asteroid.y += asteroid.dy * fElapsedTime;

           // for (int x = 0; x < asteroid.size; x++)
            //{
             //   for (int y = 0; asteroid.size; y++)
             //   {
                    //Draw(asteroid.x + x, asteroid.y + y, olc::RED);
                    // DrawRect(20.0f, 20.0f, 8, 8, olc::WHITE);
                    Draw(asteroid.x, asteroid.y, olc::WHITE);	
            //    }
           // }
        }

        return true;
    }
};

int main()
{
    Example demo;
    // construct screen
    // 160 wide 180 tall - 8 pixel per character
    if (demo.Construct(160, 180, 8, 8))  
    {
        demo.Start();
    }
    return 0;
}