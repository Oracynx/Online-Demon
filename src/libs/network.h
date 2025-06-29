#include <string>
#include <vector>
#include <windows.h>
#include <winsock2.h>
class Network
{
  private:
    SOCKET m_listenSocket = INVALID_SOCKET;
    SOCKET m_clientSocket = INVALID_SOCKET;
    bool m_isServer = false;
    bool m_initialized = false;

    // 初始化Winsock
    bool Initialize()
    {
        if (m_initialized)
            return true;

        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        {
            return false;
        }

        m_initialized = true;
        return true;
    }

    // 清理资源
    void Cleanup()
    {
        if (m_clientSocket != INVALID_SOCKET)
        {
            closesocket(m_clientSocket);
            m_clientSocket = INVALID_SOCKET;
        }

        if (m_listenSocket != INVALID_SOCKET)
        {
            closesocket(m_listenSocket);
            m_listenSocket = INVALID_SOCKET;
        }

        if (m_initialized)
        {
            WSACleanup();
            m_initialized = false;
        }
    }

  public:
    Network()
    {
    }

    ~Network()
    {
        Cleanup();
    }

    // 作为服务器运行
    bool Server(int port)
    {
        if (!Initialize())
            return false;

        // 创建监听套接字
        m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_listenSocket == INVALID_SOCKET)
        {
            return false;
        }

        // 绑定地址和端口
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(m_listenSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        {
            Cleanup();
            return false;
        }

        // 开始监听
        if (listen(m_listenSocket, 1) == SOCKET_ERROR)
        {
            Cleanup();
            return false;
        }

        m_isServer = true;
        return true;
    }

    // 等待客户端连接（仅服务器模式）
    bool Accept()
    {
        if (!m_isServer || m_listenSocket == INVALID_SOCKET)
        {
            return false;
        }

        sockaddr_in clientAddr;
        int clientSize = sizeof(clientAddr);
        m_clientSocket = accept(m_listenSocket, (sockaddr *)&clientAddr, &clientSize);

        return (m_clientSocket != INVALID_SOCKET);
    }

    // 连接到服务器
    bool Connect(const std::string &ip, int port)
    {
        if (!Initialize())
            return false;

        // 创建套接字
        m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_clientSocket == INVALID_SOCKET)
        {
            return false;
        }

        // 设置服务器地址
        sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
        serverAddr.sin_port = htons(port);

        // 连接服务器
        if (connect(m_clientSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        {
            Cleanup();
            return false;
        }

        m_isServer = false;
        return true;
    }

    // 发送消息
    bool Send(const std::string &msg)
    {
        if (m_clientSocket == INVALID_SOCKET)
        {
            return false;
        }

        int bytesSent = send(m_clientSocket, msg.c_str(), static_cast<int>(msg.size()), 0);
        return (bytesSent != SOCKET_ERROR);
    }

    // 接收消息（阻塞式）
    std::string Receive()
    {
        if (m_clientSocket == INVALID_SOCKET)
        {
            return "";
        }

        char buffer[1024];
        int bytesRead = recv(m_clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0)
        {
            return "";
        }

        buffer[bytesRead] = '\0';
        return std::string(buffer);
    }

    // 关闭连接
    void Close()
    {
        Cleanup();
    }

    // 检查是否已连接
    bool IsConnected() const
    {
        return (m_clientSocket != INVALID_SOCKET);
    }

    // 检查是否是服务器
    bool IsServer() const
    {
        return m_isServer;
    }
};