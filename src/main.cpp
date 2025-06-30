#include "core/client.cpp"
#include "core/server.cpp"
#include <iostream>
int main()
{
    std::cout << "此程序作为服务端还是客户端运行？" << std::endl;
    std::cout << "1. 服务端" << std::endl;
    std::cout << "2. 客户端" << std::endl;
    std::cout << "请输入 > ";
    int mode = 0;
    std::cin >> mode;
    if (mode == 1)
    {
        RunServer();
    }
    else if (mode == 2)
    {
        RunClient();
    }
    else
    {
        std::cout << "[ERROR] 非期望的输出\n";
    }
    return 0;
}