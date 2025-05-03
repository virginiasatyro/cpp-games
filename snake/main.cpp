//#include <algorithm>
//#include <array>
//#include <cstdint>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17
//https://github.com/Joseph21-6147/From-ConsoleGameEngine-to-PixelGameEngine/blob/main/OneLoneCoder_Snake.PGE.cpp
struct SnakeSegment
{
    float x;
    float y;
};

class SnakeGame : public olc::PixelGameEngine
{
public:
    SnakeGame()
    {
        sAppName = "Snake";
    }

private: 
    olc::vf2d headPosition;
    float speed = 20.0f;
    //enum direction {UP, RIGHT, DOWN, LEFT, NONE};
    float gunReloadDelay = 0.2f;

    std::list<SnakeSegment> snake = {{32, 32}/*, {31, 31}*/};

    void reset()
    {
        // Initial snake state
        snake = {{32, 32}/*, {32, 31}*/};
    }

public:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        //SnakeSegment snake;
        //headPosition.x = ScreenWidth() / 2.0f;
        //headPosition.y = ScreenHeight() / 2.0f;
        // Initialize game
        reset();
    
        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // INPUT ---------------------------------------------------------------------------------------------------------
       // direction dir = NONE;
        // Get input
        auto test = speed * fElapsedTime;
        if (GetKey(olc::W).bHeld || GetKey(olc::UP).bHeld)
        {
            headPosition.y -= speed * fElapsedTime;
          //  dir = UP;
            snake.push_front({snake.front().x * test, (snake.front().y - 1)  * test});
        }
        if (GetKey(olc::S).bHeld || GetKey(olc::DOWN).bHeld)
        {
            headPosition.y += speed * fElapsedTime;
           // dir = DOWN;
            snake.push_front({snake.front().x  * test, (snake.front().y + 1)  * test});
        }
        if (GetKey(olc::A).bHeld || GetKey(olc::LEFT).bHeld)
        {
            headPosition.x -= speed * fElapsedTime;
          //  dir = LEFT;
            snake.push_front({(snake.front().x - 1) * test, snake.front().y * test});
        }
        if (GetKey(olc::D).bHeld || GetKey(olc::RIGHT).bHeld)
        {
            headPosition.x += speed * fElapsedTime;
           // dir = RIGHT;
            snake.push_front({(snake.front().x + 1) * test, (snake.front().y - 1) * test});
        }

        // Spaceship cant go off screen
        /*if (headPosition.x <= 0)
        {
            headPosition.x = 0;
        }
        if (headPosition.y <= 0)
        {
            headPosition.y = 0;
        }
        if (headPosition.x + 1.0f >= (float)ScreenWidth())
        {
            headPosition.x = (float)ScreenWidth() - 1.0f; // pixel size
        }
        if (headPosition.y + 1.0f >= (float)ScreenHeight())
        {
            headPosition.y = (float)ScreenHeight() - 1.0f;
        }*/

        // Calculate where to go
       /* switch (dir)
        {
        case UP:
        {
            snake.push_front({snake.front().x, snake.front().y - 1});
            break;
        }
        case DOWN:
        {
            snake.push_front({snake.front().x, snake.front().y + 1});
            break;
        }
        case LEFT:
        {
            snake.push_front({snake.front().x - 1, snake.front().y});
            break;
        }
        case RIGHT:
        {
            snake.push_front({snake.front().x + 1, snake.front().y - 1});
            break;
        }
        case NONE:
        {
            snake.push_front({snake.front().x, snake.front().y});
            break;
        }
        default:
        {
            snake.push_front({snake.front().x, snake.front().y});
            break;
        }
        }*/

        // Snake cant go off screen
        if (snake.front().x <= 0)
        {
            snake.front().x = 0;
        }
        if (snake.front().y <= 0)
        {
            snake.front().y = 0;
        }
        if (snake.front().x + 1.0f >= (float)ScreenWidth())
        {
            snake.front().x = (float)ScreenWidth() - 1.0f; // pixel size
        }
        if (snake.front().y + 1.0f >= (float)ScreenHeight())
        {
            snake.front().y = (float)ScreenHeight() - 1.0f;
        }

        // LOGIC ---------------------------------------------------------------------------------------------------------

        // Check collision

        // DRAW ----------------------------------------------------------------------------------------------------------
        // Clear screen
        PixelGameEngine::ConsoleClear();
        Clear(olc::BLUE);

        // Draw snake
        //Draw(headPosition.x, headPosition.y, olc::GREEN);
       // for (auto &s : snake)
        //{
            Draw(snake.front().x, snake.front().y, olc::GREEN);
        //}

        // Draw fruit

        //Draw(fruit.position.x, fruit.position.y, olc::RED);

        return true;
    }
};

int main()
{
    SnakeGame demo;
    // Construct screen
    // if (demo.Construct(640, 480, 2, 2))
    if (demo.Construct(64, 64, 16, 16))
    {
        demo.Start();
    }
    return 0;
}
