#include <iostream>
#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string.h>
#include <string>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(void)
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "WSAStartup failed" << std::endl;
        return EXIT_FAILURE;
    }

    int ReturnCheck;
    SOCKET ClientId; // Use SOCKET type for Winsock
    int PortNum = 3333;
    const int MessageSize = 256;
    char Message[MessageSize];
    std::string IP = "127.0.0.1";
    sockaddr_in ServerAddr;

    // Establish connection with server.
    ClientId = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientId == INVALID_SOCKET)
    {
        std::cout << "Error creating the client socket." << std::endl;
        WSACleanup();
        return EXIT_FAILURE;
    }
    std::cout << "Client socket created." << std::endl;

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_port = htons(PortNum);
    ServerAddr.sin_addr.s_addr = inet_addr(IP.c_str());

    ReturnCheck = connect(ClientId, reinterpret_cast<struct sockaddr*>(&ServerAddr), sizeof(ServerAddr));
    if (ReturnCheck < 0)
    {
        std::cout << "Error connecting to the server." << std::endl;
        closesocket(ClientId);
        WSACleanup();
        return EXIT_FAILURE;
    }
    std::cout << "Connected to server." << std::endl;

    // Main loop.
    while (1)
    {
        std::cout << "Please enter a command: ";
        std::string Temp;
        getline(std::cin, Temp);
        strncpy_s(Message, Temp.c_str(), _TRUNCATE);

        int bytesSent = send(ClientId, Message, MessageSize, 0);
        if (bytesSent == SOCKET_ERROR)
        {
            std::cerr << "Error sending data: " << WSAGetLastError() << std::endl;
            break; // or handle the error as needed
        }

        if (!strcmp(Message, "quit"))
            break;

        // Handle connection closure by the server.
        int bytesReceived = recv(ClientId, Message, MessageSize, 0);
        if (bytesReceived == 0)
        {
            std::cout << "Connection closed by the server." << std::endl;
            break;
        }

        Message[bytesReceived] = '\0';

        std::cout << Message << std::endl;
    }

    closesocket(ClientId);
    WSACleanup();

    return 0;
}
