#include <iostream>
#include "Server.h"
#pragma comment(lib,"WS2_32")       // Provide external symbols to linker

Server* server = nullptr;

int main() {
    std::cout << " === Application Started === \n" << std::endl;

    server = new Server();

    while (server->isRunning)
    {
        if (server->init() != 0)
        {
            std::cout << "Winsock setup failed" << std::endl;
        }

        if (server->listenConnections() != 0)
        {
            std::cout << "Connections failed" << std::endl;
        }

        if (server->receiveData() != 0)
        {
            std::cout << "Receive failed" << std::endl;
        }

        if (server->closeConnection)
        {
            if (server->close() != 0)
            {
                std::cout << "Issues occurred during shutdown" << std::endl;
            }
        }
    }

    std::cout << "Closed!" << std::endl;
    return 0;
}
