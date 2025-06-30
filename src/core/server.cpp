#include "definition/game.h"
#include "libs/console.h"
#include "libs/network.h"
#include <random>
#include <vector>

std::mt19937 rnd(std::random_device{}());

// 游戏变量
class Player
{
  public:
    int health;
    bool flash;
    std::vector<int> item;

    Player()
    {
        health = HEALTH;
        flash = false;
        item.resize(MAX_PROP_COUNT, NONE);
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
};

inline void RunServer()
{
}