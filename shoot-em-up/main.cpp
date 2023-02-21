#include <algorithm>
#include <array>
#include <cstdint>

#define OLC_PGE_APPLICATION
#include "../commonlib/olcPixelGameEngine.h"

// g++ -o main.exe main.cpp -luser32 -lgdi32 -lopengl32 -lgdiplus -lShlwapi -ldwmapi -lstdc++fs -static -std=c++17

class Shmup : public olc::PixelGameEngine
{
public:
    Shmup()
    {
        sAppName = "Shoot'em up!";
    }

    olc::vf2d playerPos;
    olc::Sprite* sprPlayer = nullptr;
    olc::Sprite* sprEnemy[3];

    float playerSpeed = 100.f;

    std::array<olc::vf2d, 1000> arrStars;

    float worldSpeed = 40.0f;
    double worldPos = 0.0f;

    struct Enemy;

    struct enemyDefinition
    {
        double triggerTime = 0.0f;
        uint32_t sprID = 0;
        float health = 0.0f;
        float offset = 0.0f;

        std::function<void(Enemy&, float, float)> funcMove; // lambda
    };

    struct Enemy
    {
        olc::vf2d pos;
        enemyDefinition def;

        void update(float fElapsedTime, float scrollSpeed)
        {
            def.funcMove(*this, fElapsedTime, scrollSpeed);
        }
    };

    std::list<enemyDefinition> listSpaws;
    std::list<Enemy> listEnemies;

private:
public:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        sprPlayer = new olc::Sprite("img/Emissary.png");
        sprEnemy[0] = new olc::Sprite("img/Beholder.png");
        sprEnemy[1] = new olc::Sprite("img/Beholder.png");
        sprEnemy[2] = new olc::Sprite("img/Beholder.png");

        playerPos = {static_cast<float>(ScreenWidth()) / 2.0f, static_cast<float>(ScreenHeight()) / 2.0f};

        for (auto &star : arrStars)
        {
            star = {static_cast<float>(rand() % ScreenWidth()), static_cast<float>(rand() % ScreenHeight())};
        }

        // Movement patterns
        auto Move_None = [&](Enemy &enemy, float fElapsedTime, float scrollSpeed)
        {
            enemy.pos.y += scrollSpeed * fElapsedTime;
        };

        listSpaws =
            {
                {100.0, 0, 3.0f, 0.5f, Move_None}
            };

        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        // INPUT ---------------------------------------------------------------------------------------------------------
        if (GetKey(olc::W).bHeld || GetKey(olc::UP).bHeld)
        {
            playerPos.y -= playerSpeed * fElapsedTime;
        }
        if (GetKey(olc::S).bHeld || GetKey(olc::DOWN).bHeld)
        {
            playerPos.y += playerSpeed * fElapsedTime;
        }
        if (GetKey(olc::A).bHeld || GetKey(olc::LEFT).bHeld)
        {
            playerPos.x -= playerSpeed * fElapsedTime;
        }
        if (GetKey(olc::D).bHeld || GetKey(olc::RIGHT).bHeld)
        {
            playerPos.x += playerSpeed * fElapsedTime;
        }

        // ---------------------------------------------------------------------------------------------------------------
        worldPos += worldSpeed * fElapsedTime;

        while (!listSpaws.empty() && worldPos >= listSpaws.front().triggerTime)
        {
            Enemy e;
            e.def = listSpaws.front();
            e.pos = {listSpaws.front().offset * (float)(ScreenWidth() - sprEnemy[e.def.sprID]->width / 2.0),
                     0.0f - sprEnemy[e.def.sprID]->height
                    };
            
            listSpaws.pop_front();
            listEnemies.push_back(e);
        }

        // Update enemies
        for (auto &enemy : listEnemies)
        {
            enemy.update(fElapsedTime, worldSpeed);
        }

        // DISPLAY -------------------------------------------------------------------------------------------------------
        // Clear display
        Clear(olc::BLACK);

        // Draw stars
        for (size_t i = 0; i < arrStars.size(); i++)
        {
            auto &star = arrStars[i];

            star.y += worldSpeed * fElapsedTime * ((i < 300) ? 0.8f : 1.0f);
            if (star.y > static_cast<float>(ScreenHeight()))
            {
                star = {static_cast<float>(rand() % ScreenWidth()), 0.0f};
            }

            Draw(star, ((i < 300) ? olc::VERY_DARK_GREY : olc::WHITE));
        }

        // Draw spaceship
        // SetPixelMode(olc::Pixel::MASK); // allow mask - transparency
        DrawSprite(playerPos, sprPlayer);

        for (auto &enemy : listEnemies)
        {
            DrawSprite(enemy.pos, sprEnemy[enemy.def.sprID]);
        }

        // SetPixelMode(olc::Pixel::NORMAL);

        return true;
    }
};

int main()
{
    Shmup demo;
    // construct screen
    if (demo.Construct(640, 480, 2, 2))
    {
        demo.Start();
    }
    return 0;
}
