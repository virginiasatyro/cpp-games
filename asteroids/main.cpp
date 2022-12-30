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
        //vecAsteroids.push_back({20.0f, 20.0f, 8.0f, -6.0f, /*static_cast<int>(*/16/*)*/});
        vecAsteroids.push_back({20.0f, 20.0f, 8.0f, -6.0f, (int)16});
        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // clear console
        PixelGameEngine::ConsoleClear();

        // update and draw asteroids
        for (auto &a : vecAsteroids)
        {
            a.x += a.dx * fElapsedTime; // fElapsedTime - time between frames
            a.y += a.dy * fElapsedTime;
            wrapCoordinates(a.x, a.y, a.x, a.y);
            //for (int x = 0; x < a.size; x++)
            //{
               // for (int y = 0; a.size; y++)
                //{
                 //   Draw(a.x + x, a.y + y, olc::RED);
                    //  DrawRect(20.0f, 20.0f, 8, 8, olc::GREEN);
                    Draw(a.x, a.y, olc::BLUE);
           // }
        }

        // Called once per frame, draws random coloured pixels
		//for (int _x = 0; _x < ScreenWidth(); _x++)
		//	for (int _y = 0; _y < ScreenHeight(); _y++)
		//		Draw(_x, _y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));

        return true;
    }

    // toroidal mapping
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
            iy + (float)ScreenHeight();
        }
        if (iy >= (float)ScreenHeight())
        {
            oy = iy - (float)ScreenHeight();
        }
    }
};

int main()
{
    Asteroids demo;
    // construct screen
    // 160 wide 180 tall - 8 pixel per character
    if (demo.Construct(160, 180, 8, 8))  
    //	if (demo.Construct(256, 240, 4, 4))
    {
        demo.Start();
    }
    return 0;
}

/*
// Override base class with your custom functionality
class Example : public olc::PixelGameEngine
{
public:
	Example()
	{
		// Name your application
		sAppName = "Example";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		for (int x = 0; x < ScreenWidth(); x++)
			for (int y = 0; y < ScreenHeight(); y++)
				Draw(x, y, olc::Pixel(rand() % 256, rand() % 256, rand() % 256));
		return true;
	}
};

int main()
{
	Example demo;
	if (demo.Construct(256, 240, 4, 4))
		demo.Start();
	return 0;
}
*/