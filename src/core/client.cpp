#include "definition/game.h"
#include "libs/console.h"
#include "libs/network.h"
#include <iostream>
#include <string>

std::string ip;

Network client;

inline void RunClient()
{
    Console::ClearScreen();
    std::cout << "输入目标服务器地址：";
    std::cin >> ip;
    client.Connect(ip, PORT);
    for (;;)
    {
        std::string command = client.Receive();
        std::string type = command.substr(0, command.find("<$>"));
        if (type == "FLUSH")
        {
            std::string screen = command.substr(command.find("<$>") + 3);
            Console::ClearScreen();
            std::cout << screen;
        }
        else if (type == "INPUT")
        {
            std::string user;
            std::cin >> user;
            client.Send(user);
        }
        else if (type == "END")
        {
            break;
        }
    }
}