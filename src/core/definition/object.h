#include "game.h"
#include <random>
#include <vector>

#ifndef OBJECT_DEFINITION

#define OBJECT_DEFINITION

// 游戏变量
class Player
{
  public:
    int health;
    bool skip;
    bool death;
    bool flash;
    bool skipUsed;
    std::vector<int> item;
    std::mt19937 rnd;
    Player()
    {
        rnd = std::mt19937(std::random_device{}());
        health = HEALTH;
        death = false;
        flash = false;
        skip = true;
        skipUsed = false;
        item.resize(MAX_PROP_COUNT, NONE);
    }
    void NewTurn()
    {
        skipUsed = false;
    }
    void Reloading()
    {
        skip = false;
        skipUsed = false;
    }
    void getItem()
    {
        auto it = item.begin();
        for (int i = 0; i < PROP_COUNT_GET_ONCE; i++)
        {
            for (; it != item.end() && *it == NONE;)
            {
                it++;
            }
            *it = 7001 + (rnd() % PROP_KIND);
        }
    }
    void Hurt(int x)
    {
        if (flash)
        {
            death = true;
            return;
        }
        health -= x;
        if (health <= FLASH_HEALTH)
        {
            flash = true;
            health = -1;
        }
    }
    void Help(int x)
    {
        if (flash)
        {
            return;
        }
        health = std::min(health + x, HEALTH);
    }
};

class Gun
{
  public:
    int live;
    int blank;
    bool saw;
    std::vector<bool> v;
    std::mt19937 rnd;
    Gun()
    {
        live = 0;
        blank = 0;
        saw = false;
        rnd = std::mt19937(std::random_device{}());
    }
    void generate(int size)
    {
        live = 0;
        blank = 0;
        v.clear();
        for (int i = 0; i < size; i++)
        {
            if (rnd() % 2)
            {
                live++;
                v.push_back(true);
            }
            else
            {
                blank++;
                v.push_back(false);
            }
        }
    }
    void pop()
    {
        v.erase(v.begin());
    }
};

#endif