#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define DEFAULT_PORT "54321"
#define DEFAULT_BUFLEN 256

int main() {
    std::cout << "Server starting ... ";

    WSADATA wsaData;
    int iResult;

    char recvbuf[DEFAULT_BUFLEN];       // char buffer
    int iSendResult;
    int recvbuflen = DEFAULT_BUFLEN;

    int messageCount = 0;

    struct addrinfo *result = NULL;
    struct addrinfo hints;

    SOCKET listenSocket = INVALID_SOCKET;
    SOCKET clientSocket = INVALID_SOCKET;

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

    // Create server socket
    listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (listenSocket == INVALID_SOCKET)
    {
        printf("\nError at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    // Setup server socket to listen for connections
    iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR)
    {
        printf("\nBind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Waiting for connection ... ";

    // Once bind is complete addrinfo from result is no longer required
    freeaddrinfo(result);

    if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
        printf("\nListen failed with error: %ld\n", WSAGetLastError());
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // Accept a socket connection from client
    clientSocket = accept(listenSocket, NULL, NULL);
    if (clientSocket == INVALID_SOCKET)
    {
        printf("\naccept failed: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected!" << std::endl;

    // Close listen socket once connection has been made
    closesocket(listenSocket);

    // Receive until client closes connection
    do {
        messageCount++;

        iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {

            // Echo buffer back to sender
            iSendResult = send(clientSocket, recvbuf, iResult, 0);
            if (iSendResult == SOCKET_ERROR) {
                printf("\nsend failed: %d\n", WSAGetLastError());
                closesocket(clientSocket);
                WSACleanup();
                return 1;
            }

            std::cout << "========================================" << std::endl;
            std::cout << "Message [" << messageCount << "]:" << std::endl;

            // Print out recvbuf to console
            for(int i = 0; i < iResult; i++)
            {
                std::cout << recvbuf[i];
            }

            std::cout << "\n";
            printf("Bytes received : %d, ", iResult);
            printf("Bytes sent: %d\n", iSendResult);
            std::cout << "========================================" << std::endl;

        } else if (iResult == 0) {
            printf("Connection closing ... ");
        } else {
            printf("recv failed: %d\n", WSAGetLastError());
            closesocket(clientSocket);
            WSACleanup();
            return 1;
        }
    } while (iResult > 0);

    // Shutdown connections to clients
    iResult = shutdown(clientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("\nshutdown failed with error: %d\n", WSAGetLastError());
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    // Close client socket connection
    closesocket(clientSocket);
    WSACleanup();

    std::cout << "Closed!" << std::endl;
    return 0;
}
