#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <winsock2.h>

#include "Commands.h"
#include "Database.h"

int main(void)
{
    Database Db;
    Commands Ops;
    const int CommandSize = 256;
    char Command[CommandSize];
    SOCKET ServerId, ClientId;  // Use SOCKET type for Winsock
    int PortNum = 3333;
    struct sockaddr_in ServerAddr;
    int addrLen;

    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cout << "Failed to initialize Winsock." << std::endl;
        return EXIT_FAILURE;
    }

    // Initialize database.
    Db.InitDatabase();

    // Establish connection with client.
    ClientId = socket(AF_INET, SOCK_STREAM, 0);
    if (ClientId == INVALID_SOCKET)
    {
        std::cout << "Error creating the client socket." << std::endl;
        WSACleanup();
        return EXIT_FAILURE;
    }
    std::cout << "Client socket created." << std::endl;

    ServerAddr.sin_family = AF_INET;
    ServerAddr.sin_addr.s_addr = INADDR_ANY;
    ServerAddr.sin_port = htons(PortNum);

    addrLen = sizeof(ServerAddr);
    if (bind(ClientId, reinterpret_cast<struct sockaddr*>(&ServerAddr), addrLen) == SOCKET_ERROR)
    {
        std::cout << "Error binding the client socket." << std::endl;
        closesocket(ClientId);
        WSACleanup();
        return EXIT_FAILURE;
    }
    std::cout << "Waiting for client..." << std::endl;

    listen(ClientId, 1);
    ServerId = accept(ClientId, reinterpret_cast<struct sockaddr*>(&ServerAddr), &addrLen);
    if (ServerId == INVALID_SOCKET)
    {
        std::cout << "Error accepting the client." << std::endl;
        closesocket(ClientId);
        WSACleanup();
        return EXIT_FAILURE;
    }
    std::cout << "Client connected." << std::endl;

    Ops.CommandSize = CommandSize;
    Ops.ServerId = ServerId;

    // Main server loop.
    while (1)
    {
        // Get command from client.
        addrLen = recv(ServerId, Command, CommandSize, 0);
        if (addrLen == 0)
        {
            std::cout << "Error receiving message." << std::endl;
            break;
        }

        if (!strcmp(Command, "quit"))
            break;

        Ops.ParseCommand(Db, std::string(Command));
    }

    closesocket(ServerId);
    closesocket(ClientId);
    WSACleanup();

    return 0;
}
