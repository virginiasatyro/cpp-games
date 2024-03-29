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

    float playerSpeed = 100.0f;
    float playerHealth = 100.0f;

    float worldSpeed = 40.0f;
    double worldPos = 0.0f;

    std::array<olc::vf2d, 1000> arrStars;

    float gunReloadTimer = 0.0f;
    float gunReloadDelay = 0.2f;

    bool canFire = false;

    struct Bullet
    {
        olc::vf2d pos;
        olc::vf2d vel; // velocity - speed and direction
        bool remove = false;
    };

    struct Enemy;

    struct enemyDefinition
    {
        double triggerTime = 0.0f;
        uint32_t sprID = 0;
        float health = 100.0f;
        float offset = 0.0f;

        std::function<void(Enemy&, float, float)> funcMove; // lambda
        std::function<void(Enemy&, float, float, std::list<Bullet>&)> funcFire;
    };

    struct Enemy
    {
        olc::vf2d pos;
        enemyDefinition def;

        std::array<float, 4> dataMove{0};
        std::array<float, 4> dataFire{0};

        void update(float fElapsedTime, float scrollSpeed, std::list<Bullet> &b)
        {
            def.funcMove(*this, fElapsedTime, scrollSpeed);
            def.funcFire(*this, fElapsedTime, scrollSpeed, b);
        }
    };

    std::list<enemyDefinition> listSpaws;
    std::list<Enemy> listEnemies;
    std::list<Bullet> listBullets;
    std::list<Bullet> listPlayerBullets;
    std::list<Bullet> listFragments;

private:
public:
    // called once at the start, so create things here
    bool OnUserCreate() override
    {
        sprPlayer = new olc::Sprite("img/blue_01.png");
        sprEnemy[0] = new olc::Sprite("img/red_01.png");
        sprEnemy[1] = new olc::Sprite("img/orange_03.png");
        sprEnemy[2] = new olc::Sprite("img/green_06.png");

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

        auto Move_Fast = [&](Enemy &enemy, float fElapsedTime, float scrollSpeed)
        {
            enemy.pos.y += scrollSpeed * fElapsedTime * 3.0f;
        };

        auto Move_SinusoidNarrow = [&](Enemy &enemy, float fElapsedTime, float scrollSpeed)
        {
            enemy.pos.y += scrollSpeed * fElapsedTime * 1.0f;
            enemy.dataMove[0] += fElapsedTime;
            enemy.pos.x += 50.0f * cosf(enemy.dataMove[0]) * fElapsedTime;
        };

        auto Move_SinusoidWide = [&](Enemy &enemy, float fElapsedTime, float scrollSpeed)
        {
            enemy.pos.y += scrollSpeed * fElapsedTime * 1.0f;
            enemy.dataMove[0] += fElapsedTime;
            enemy.pos.x += 200.0f * cosf(enemy.dataMove[0]) * fElapsedTime;
        };

        // Firing patterns
        auto Fire_None = [&](Enemy &enemy, float fElapsedTime, float scrollSpeed, std::list<Bullet> &bullets)
        {
            //
        };

        auto Fire_Straight2 = [&](Enemy &enemy, float fElapsedTime, float scrollSpeed, std::list<Bullet> &bullets)
        {
            constexpr float delay = 0.8f;
            enemy.dataFire[0] += fElapsedTime;
            if(enemy.dataFire[0] >= delay)
            {
                enemy.dataFire[0] -= delay;
                Bullet b;
                b.pos = enemy.pos + olc::vf2d(24, 48); // depends on sprites size
                b.vel = {0.0f, 180.0f};
                bullets.push_back(b);
            }
        };

        auto Fire_CirclePulse2 = [&](Enemy &enemy, float fElapsedTime, float scrollSpeed, std::list<Bullet> &bullets)
        {
            constexpr float delay = 2.0f;
            constexpr int nBullets = 10;
            constexpr float fTheta = 3.14159f * 2.0f / (float)nBullets;
            enemy.dataFire[0] += fElapsedTime;

            if(enemy.dataFire[0] >= delay)
            {
                enemy.dataFire[0] -= delay;
                for (int i = 0; i < nBullets; i++)
                {
                    Bullet b;
                    b.pos = enemy.pos + olc::vf2d(24, 24);
                    b.vel = {180.0f * cos(fTheta * i), 180.0f * sinf(fTheta * i)};
                    bullets.push_back(b);
                }
            }
        };

        listSpaws =
            {
                {60.0, 0, 3.0f, 0.5f, Move_SinusoidWide, Fire_CirclePulse2},
                {240.0, 1, 3.0f, 0.25f, Move_SinusoidNarrow, Fire_Straight2},
                {240.0, 1, 3.0f, 0.75f, Move_SinusoidNarrow, Fire_Straight2},
                {360.0, 2, 3.0f, 0.25f, Move_None, Fire_Straight2},
                {360.0, 2, 3.0f, 0.5f, Move_None, Fire_None},
                {360.0, 2, 3.0f, 0.75f, Move_None, Fire_Straight2},
            };

        return true;
    }

    // called once per frame
    bool OnUserUpdate(float fElapsedTime) override
    {
        playerPos.y += worldSpeed * fElapsedTime;
        // INPUT ---------------------------------------------------------------------------------------------------------
        if (GetKey(olc::W).bHeld || GetKey(olc::UP).bHeld)
        {
            playerPos.y -= (playerSpeed + worldSpeed) * fElapsedTime;
        }
        if (GetKey(olc::S).bHeld || GetKey(olc::DOWN).bHeld)
        {
            playerPos.y += (playerSpeed - worldSpeed) * fElapsedTime;
        }
        if (GetKey(olc::A).bHeld || GetKey(olc::LEFT).bHeld)
        {
            playerPos.x -= playerSpeed * fElapsedTime * 2.0f;
        }
        if (GetKey(olc::D).bHeld || GetKey(olc::RIGHT).bHeld)
        {
            playerPos.x += playerSpeed * fElapsedTime * 2.0f;
        }

        // Spaceship cant go off screen
        if(playerPos.x <= 0)
        {
            playerPos.x = 0;
        }
        if(playerPos.y <= 0)
        {
            playerPos.y = 0;
        }
        if(playerPos.x + 48.0f >= (float)ScreenWidth())
        {
            playerPos.x = (float)ScreenWidth() - 48.0f;
        }
        if(playerPos.y + 48.0f >= (float)ScreenHeight())
        {
            playerPos.y = (float)ScreenHeight()- 48.0f;
        }

        gunReloadTimer += fElapsedTime;
        if(gunReloadTimer >= gunReloadDelay)
        {
            canFire = true;
            gunReloadTimer -= gunReloadDelay;
        }

        if(GetKey(olc::SPACE).bHeld || GetMouse(0).bHeld)
        {
            if(canFire)
            {
                Bullet b;
                b.pos = {playerPos.x + 24.0f, playerPos.y};
                b.vel = {0.0, -200.0f};
                listPlayerBullets.push_back(b);
                canFire = false;
            }
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

        // Update fragments - behave like bullets
        for(auto &b : listFragments)
        {
            b.pos += (b.vel + olc::vf2d(0.0f, worldSpeed)) * fElapsedTime;
        }

        // Update enemies
        for (auto &enemy : listEnemies)
        {
            enemy.update(fElapsedTime, worldSpeed, listBullets);
        }

        // Update bullets
        for(auto &b : listBullets)
        {
            b.pos += (b.vel + olc::vf2d(0.0f, worldSpeed)) * fElapsedTime;

            // circle x circle collision
            if ((b.pos - (playerPos + olc::vf2d(24, 24))).mag2() < 24.0f * 24.0f) // player position = top left of the sprite
            { 
                b.remove = true;
                playerHealth -= 10.0f;
            }
        }

        // Update player bullets
        for (auto &b : listPlayerBullets)
        {
            b.pos += (b.vel + olc::vf2d(0.0f, worldSpeed)) * fElapsedTime;

            // Check eneies vs player bullets
            for (auto &e : listEnemies)
            {
                if ((b.pos - (e.pos + olc::vf2d(24, 24))).mag2() < 24.0f * 24.0f)
                {
                    b.remove = true;
                    e.def.health -= 25.0f; // one hit and enemy is off

                    // enemy destroyed
                    if (e.def.health <= 0)
                    {
                        for (int i = 0; i < 500; i++)
                        {
                            float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f;
                            float speed = ((float)rand() / (float)RAND_MAX) * 200.0f + 50.0f;
                            listFragments.push_back(
                                {
                                    e.pos + olc::vf2d(24, 24), // start explosion in the middle of enemy sprite
                                    {
                                        speed * cosf(angle), speed * sinf(angle)
                                    }
                                }
                            );
                        }
                    }
                }
            }
        }

        // Remove enemies that are redundant
        listEnemies.remove_if([&](const Enemy &e)
                              { return (e.pos.y >= (float)ScreenHeight()) || e.def.health <= 0.0f; });

        // Remove bullets off screen
        listBullets.remove_if([&](const Bullet &b)
                              { return (b.pos.x < 0 || b.pos.x > (float)ScreenWidth() || b.pos.y > (float)ScreenHeight() || b.pos.y < 0 || b.remove); });

        // Remove player bullets off screen
        listPlayerBullets.remove_if([&](const Bullet &b)
                              { return (b.pos.x < 0 || b.pos.x > (float)ScreenWidth() || b.pos.y > (float)ScreenHeight() || b.pos.y < 0 || b.remove); });

        // Remove fragments
        listFragments.remove_if([&](const Bullet &b)
                              { return (b.pos.x < 0 || b.pos.x > (float)ScreenWidth() || b.pos.y > (float)ScreenHeight() || b.pos.y < 0 || b.remove); });

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
        SetPixelMode(olc::Pixel::MASK); // allow mask - transparency
        DrawSprite(playerPos, sprPlayer);

        for (auto &enemy : listEnemies)
        {
            DrawSprite(enemy.pos, sprEnemy[enemy.def.sprID]);
        }

        SetPixelMode(olc::Pixel::NORMAL);

        // Draw bullets
        for (auto b : listBullets)
        {
            FillCircle(b.pos, 3, olc::RED);
        }

        // Draw player bullets
        for (auto b : listPlayerBullets)
        {
            FillCircle(b.pos, 3, olc::CYAN);
        }

        // Draw fragments
        for( auto&b : listFragments)
        {
            Draw(b.pos, olc::YELLOW);
        }

        // Draw HUD
        DrawString(4, 4, "HEALTH:");
        FillRect(60, 4, (playerHealth / 100.0f * 576.0f), 8, olc::GREEN);

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
