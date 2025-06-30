#include "definition/game.h"
#include "libs/console.h"
#include "libs/network.h"
#include <algorithm>
#include <format>
#include <iostream>
#include <random>
#include <string>
#include <synchapi.h>
#include <tuple>
#include <vector>

std::mt19937 rnd(std::random_device{}());

// 游戏变量
class Player
{
  public:
    int health;
    bool death;
    bool flash;
    bool sawUsed;
    std::vector<int> item;
    Player()
    {
        health = HEALTH;
        death = false;
        flash = false;
        sawUsed = false;
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
    Gun()
    {
        live = 0;
        blank = 0;
        saw = false;
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
};

Gun gun;
Player A, B;
Network client;

std::vector<std::tuple<std::string, int>> systemMessage;

void addSystemMessage(std::string msg, int role)
{
    systemMessage.insert(systemMessage.begin(), {msg, role});
    while (systemMessage.size() > MAX_MESSAGE_HISTORY)
    {
        systemMessage.pop_back();
    }
}

bool RunCommand(const Player &A, const Player &B, std::string command, int roleA, int roleB)
{
    std::string type, arg;
    type = command.substr(0, command.find(" "));
    arg = command.substr(command.find(" "));
    if (type == "shoot")
    {
        if (arg != "self" && arg != "other")
        {
            addSystemMessage("开枪参数错误，需要 [self/other]", roleA);
            return false;
        }
    }
    else if (type == "item")
    {
        int id = std::stoi(arg);
        int size = A.item.size();
        id--;
        if (!(0 <= id && id < size))
        {
            addSystemMessage(std::format("道具参数错误，需要 [{}~{}]", 1, size), roleA);
            return false;
        }
    }
    else
    {
        addSystemMessage("未知命令", roleA);
        return false;
    }
}

void RunServer()
{
    std::cout << std::format("在端口 {} 开启服务", PORT) << std::endl;
    client.Server(PORT);
    std::cout << "等待客户端连接" << std::endl;
    client.Accept();
    std::cout << "客户端连接成功" << std::endl;
    Sleep(1000);
    for (;;)
    {
        while (true)
        {
            std::string command;
            std::getline(std::cin, command);
        }
    }
}