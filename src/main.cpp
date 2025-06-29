#include "core/client.h"
#include "core/server.h"
#include <cstdio>
int main()
{
    printf("此程序作为服务端还是客户端运行？\n");
    printf("1. 服务端\n");
    printf("2. 客户端\n");
    printf("请输入：");
    int mode = 0;
    scanf("%d", &mode);
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
        printf("[ERROR] 非期望的输出\n");
    }
    return 0;
}