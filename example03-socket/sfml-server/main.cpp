#include <SFML/Network.hpp>
#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>

std::vector<std::shared_ptr<sf::TcpSocket>> clients;
std::mutex clientsMutex;

void handleClient(std::shared_ptr<sf::TcpSocket> client)
{
    sf::Packet packet;
    std::string message;

    while (true)
    {
        sf::Socket::Status status = client->receive(packet);
        if (status == sf::Socket::Done)
        {
            packet >> message;
            packet.clear();

            std::cout << "收到客户端消息: " << message << std::endl;

            // 广播给所有客户端
            std::lock_guard<std::mutex> lock(clientsMutex);
            for (auto &c : clients)
            {
                if (c != client)
                {
                    sf::Packet sendPacket;
                    sendPacket << message;
                    c->send(sendPacket);
                }
            }
        }
        else if (status == sf::Socket::Disconnected)
        {
            std::cout << "客户端断开连接。" << std::endl;
            break;
        }
        else
        {
            break;
        }
    }

    // 从列表移除
    std::lock_guard<std::mutex> lock(clientsMutex);
    clients.erase(std::remove(clients.begin(), clients.end(), client), clients.end());
}

int main()
{
    sf::TcpListener listener;
    if (listener.listen(54000) != sf::Socket::Done)
    {
        std::cerr << "服务器监听失败！" << std::endl;
        return -1;
    }

    std::cout << "服务器启动成功，监听端口 54000..." << std::endl;

    while (true)
    {
        auto client = std::make_shared<sf::TcpSocket>();
        if (listener.accept(*client) == sf::Socket::Done)
        {
            std::cout << "新客户端连接：" << client->getRemoteAddress() << std::endl;
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                clients.push_back(client);
            }
            std::thread(&handleClient, client).detach();
        }
    }

    return 0;
}

