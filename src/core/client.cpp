#include "definition/game.h"
#include "definition/object.h"
#include "libs/console.h"
#include "libs/network.h"
#include <format>
#include <string>
#include <tuple>
#include <vector>

Gun gun;
Player me;

std::vector<std::tuple<std::string, int>> systemMessage;

std::string getInter(const std::string &str, int index)
{
    std::string result;
    for (; index < int(str.size()) && std::isdigit(str[index]);)
    {
        result += str[index];
        ++index;
    }
    return result;
}

void addSystemMessage(std::string msg, int role)
{
    systemMessage.insert(systemMessage.begin(), {msg, role});
    while (systemMessage.size() > MAX_MESSAGE_HISTORY)
    {
        systemMessage.pop_back();
    }
}

std::string command(std::string cmd)
{
    // 修改操作
    if (cmd.find("c|game|shoot|") != std::string::npos)
    {
        int k = std::stoi(getInter(cmd, 13));
        me.Hurt(k);
        return "";
    }
    else if (cmd.find("c|game|item|") != std::string::npos)
    {
        int item = std::stoi(getInter(cmd, 12));
        if (item == BEER)
        {
            gun.pop();
        }
        return "";
    }
    else if (cmd == "c|item|generate|")
    {
        // 生成道具的实现
        me.getItem(); // 直接调用getItem方法
        return "";
    }
    else if (cmd.find("c|item|remove|") != std::string::npos)
    {
        int itemIndex = std::stoi(getInter(cmd, 14));
        // 检查索引范围
        if (itemIndex >= 0 && itemIndex < int(me.item.size()))
        {
            me.item[itemIndex] = NONE; // 将道具设为NONE表示移除
        }
        else
        {
            addSystemMessage(std::format("无效的道具索引: {}", itemIndex), ROLE_SERVER);
        }
        return "";
    }
    else if (cmd == "c|none||")
    {
        return "";
    }
    // 查询操作
    else if (cmd == "q|player|health")
    {
        return std::to_string(me.health); // 直接访问health成员
    }
    else if (cmd == "q|player|item")
    {
        std::string items;
        for (int id : me.item)
        {
            if (id != NONE)
            { // 只返回非空道具
                if (!items.empty())
                    items += ";";
                items += std::to_string(id);
            }
        }
        return items;
    }
    else if (cmd == "q|game|gun")
    {
        // 返回枪支状态: 是否使用过手锯;实弹数;空包弹数
        return std::format("{:d};{:d};{:d}", gun.saw, gun.live, gun.blank);
    }
    // 未知命令处理
    else
    {
        addSystemMessage("未知命令: " + cmd, ROLE_SERVER);
        return "ERROR: Unknown command";
    }
}

void send(Network, std::string);
void receive(Network);

void send(Network &client, std::string msg)
{
    if (msg[0] == 'q')
    {
        client.Send(msg);
    }
    else
    {
    }
    // Clear system messages after sending
    systemMessage.clear();
}

bool receive(Network &client)
{
    std::string msg = client.Receive();
    if (msg == "END")
    {
        return true;
    }
    command(msg);
}

bool RunCommand(std::string command, Network &client)
{
    std::string type, arg;
    type = command.substr(0, command.find(" "));
    arg = command.substr(command.find(" "));
    if (type == "shoot")
    {
        if (arg != "self" && arg != "other")
        {
            addSystemMessage("开枪参数错误，需要 [self/other]", ROLE_SERVER);
            return false;
        }
    }
    else if (type == "item")
    {
        int id = std::stoi(arg);
        int size = me.item.size();
        id--;
        if (!(0 <= id && id < size))
        {
            addSystemMessage(std::format("道具参数错误，需要 [{}~{}]", 1, size), ROLE_SERVER);
            return false;
        }
    }
    else
    {
        addSystemMessage("未知命令", ROLE_SERVER);
        return false;
    }
}

void RunClient(Network &client)
{
    Console::ClearScreen();
}
