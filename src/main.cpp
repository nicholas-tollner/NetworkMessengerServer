#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define DEFAULT_PORT "54321"

 // Create 3 addrinfo structs from ws2tcpip.h
struct addrinfo *result = NULL, *ptr = NULL, hints;

int main() {
    std::cout << "Server starting ... " << std::endl;

    // --- Flags to specify connection type ---
    // AF_LOCAL for communication between processes on same host
    // AF_INET for IPv4 - 32-bit address / 4 bytes
    // AF_INET6 for IPv6 - 128-bit address / 16 bytes

    // -- Flags to specify communication type ---
    // TCP - Transmission control protocol
    // UDP - User datagram protocol -> connectionless

    WSADATA wsaData;
    int iResult;

    /**
     * WSAStartup initiates Winsock DLL
     * MAKEWORD concatenates two 1-byte words into one 16-bit word
     * MAKEWORD requests version 2.2 of Winsock
     * If iResult != 0, WSAStartup has failed
     */
    iResult = WSAStartup(MAKEWORD(2,2), &wsaData);

    if (iResult != 0)
    {
        printf("WSAStartup failed: %d\n", iResult);
        return 1;
    }

    // Fills a block of memory with zeroes
    ZeroMemory(&hints, sizeof (hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;        // Specify TCP protocol
    hints.ai_flags = AI_PASSIVE;            // PASSIVE refers t

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0)
    {
        printf("getaddrinfo failed: %d\n", iResult);
        WSACleanup();                       // Terminates Winsock2 DLL
        return 1;
    }

    SOCKET ListenSocket = INVALID_SOCKET;
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);

    if (ListenSocket == INVALID_SOCKET)
    {
        printf("Error at socket(): %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }


    std::cout << "Closing ..." << std::endl;
    return 0;
}
