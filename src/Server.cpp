//
// Created by Nickt on 18/05/2022.
//

#include "Server.h"
#include <stdio.h>
#include <iostream>
#include <thread>

Server::Server()
{
    SOCKET clientSocket[] = {INVALID_SOCKET, INVALID_SOCKET};       // Array to store active connections
}

int Server::init() {
    // Initialise Winsock
    /**
     * WSAStartup initiates Winsock DLL
     * MAKEWORD concatenates two 1-byte words into one 16-bit word
     * MAKEWORD requests version 2.2 of Winsock
     * If iResult != 0, WSAStartup has failed
     */
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0)
    {
        printf("\nWSAStartup failed: %d\n", iResult);
        return 1;
    }

    // Fills a block of memory with zeroes
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;        // Specify TCP protocol
    hints.ai_flags = AI_PASSIVE;            // PASSIVE indicates socket address structure will be used in bind function

    // Source IP Address and port
    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("\ngetaddrinfo failed: %d\n", iResult);
        WSACleanup();                       // Terminates Winsock2 DLL
        return 1;
    }

    return 0;
}

int Server::listenConnections() {
    connections = 0;
    // Setup server socket to listen for connections
    while (connections < MAX_CONNECTIONS) {
        std::cout << "Setting up socket ... ";
        // Create server socket
        listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (listenSocket == INVALID_SOCKET) {
            printf("\nError at socket(): %ld\n", WSAGetLastError());
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }
        std::cout << "Success!" << std::endl;

        std::cout << "Attempting bind ... ";
        iResult = bind(listenSocket, result->ai_addr, (int) result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            printf("\nBind failed with error: %d\n", WSAGetLastError());
            freeaddrinfo(result);
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }
        std::cout << "Success!" << std::endl;

        // Once bind is complete addrinfo from result is no longer required
        freeaddrinfo(result);

        std::cout << "Attempting listen ... ";
        if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
            printf("\nListen failed with error: %ld\n", WSAGetLastError());
            closesocket(listenSocket);
            WSACleanup();
            return 1;
        }
        std::cout << "Success!" << std::endl;

        // Accept a socket connection from client
        std::cout << "Accepting connection ... ";
        clientSocket[connections] = accept(listenSocket, NULL, NULL);
        if (clientSocket[connections] == INVALID_SOCKET) {
            printf("\naccept failed: %d\n", WSAGetLastError());
            closesocket(clientSocket[connections]);
            WSACleanup();
            return 1;
        }
        std::cout << "Success!\n" << std::endl;
        connections++;


        // Close listen socket once connection has been made
        closesocket(listenSocket);
    }

    return 0;
}

int Server::receiveData() {
    std::thread client_1([this] { receiveClient(1);});

    client_1.join();

    return 0;
}

int Server::receiveClient(int client_no)
{
    SOCKET tmp_client = clientSocket[client_no - 1];
    // Receive until client closes connection
    do {
        iResult = recv(tmp_client, recvbuf, recvbuflen, 0);

        if (iResult > 0) {
            if (client_no == 1)
            {
                printf("Bytes received : %d\n", iResult);

                // Print out message to console
                std::cout << "Message: ";
                for (int i = 0; i < iResult; i++)
                {
                    std::cout << recvbuf[i];
                }

                std::thread send_thread([this] {sendData(0); });
                send_thread.join();
            }
        } else if (iResult == 0) {
            printf("Connection closing ... ");
        } else {
            std::cout << " === Client [" << client_no << "] Disconnected === " << std::endl;
            closesocket(tmp_client);
            WSACleanup();
            return 1;
        }
    } while(iResult > 0);
}

int Server::sendData(int target_index) {

    iSendResult = send(clientSocket[target_index], sendbuf, iResult, 0);
    if (iSendResult == SOCKET_ERROR) {
        printf("\nsend failed: %d\n", WSAGetLastError());
        closesocket(clientSocket[target_index]);
        WSACleanup();
        return 1;
    }
    std::cout << "\nReturned " << iSendResult << " bytes to client!\n" << std::endl;
}

int Server::close() {
    // Shutdown connections to clients
    iResult = shutdown(clientSocket[0], SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("\nshutdown failed with error: %d\n", WSAGetLastError());
        closesocket(clientSocket[0]);
        WSACleanup();
        return 1;
    }

    // Close client socket connection
    closesocket(clientSocket[0]);
    WSACleanup();

    return 0;
}
