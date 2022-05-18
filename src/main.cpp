#include <iostream>
#include "Server.h"

Server *server = nullptr;

int main() {
    std::cout << "Server starting ... ";

    server = new Server();

    if (server->init() != 0)
    {
        std::cout << "Server failed to start" << std::endl;
    }

    if (server->listenConnections() != 0)
    {
        std::cout << "Connections failed" << std::endl;
    }

    std::cout << "Connections Complete!" << std::endl;

    if (server->receiveData() != 0)
    {
        std::cout << "Receive failed" << std::endl;
    }

    if (server->close() != 0)
    {
        std::cout << "Issues occurred during shutdown" << std::endl;
    }

    std::cout << "Closed!" << std::endl;
    return 0;
}
