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