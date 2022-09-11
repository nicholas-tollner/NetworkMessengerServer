//
// Created by Nickt on 18/05/2022.
//

#ifndef NETWORKMESSENGERSERVER_SERVER_H
#define NETWORKMESSENGERSERVER_SERVER_H

#define DEFAULT_PORT "54321"
#define DEFAULT_BUFLEN 256
#define MAX_CONNECTIONS 1

#include <winsock2.h>
#include <ws2tcpip.h>

class Server {
public:
    Server();
    ~Server();

    int init();
    int listenConnections();
    int receiveData();
    int receiveClient(int client_no);
    int sendData(int target_index);
    int close();

    bool isRunning = true;
    bool closeConnection = false;

    WSADATA wsaData;
    int iResult;

    char recvbuf[DEFAULT_BUFLEN];       // char buffer
    char sendbuf[DEFAULT_BUFLEN] = {'P', 'o', 'n', 'g', '!'};       // Returned after ping

    int iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;
    int connections = 0;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket[];
};

#endif //NETWORKMESSENGERSERVER_SERVER_H
