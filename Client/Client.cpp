// EmailClient.cpp
#include <iostream>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

using namespace std;

bool isLogged = false;

void commandHelperFunction(string command, SOCKET clientSocket, char* buffer);

int main() {
    // Initialize WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Failed to initialize WinSock." << endl;
        return 1;
    }

    // Create a socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // Set up the server address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(27015);  // Example port number
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to the server
    if (connect(clientSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        cout << "Connection failed: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    cout << "Connected to Email Server." << endl;
    
    while (1) {
        char buffer[BUFFER_SIZE];

        if (isLogged == false) {
            int iResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);

            if (iResult == SOCKET_ERROR) {
                cerr << "Receive failed" << WSAGetLastError() << endl;
            }
            else {
                string responseMessage(buffer, iResult);
                // Server trazi da se korisnik loguje
                cout << responseMessage << endl;

                string login;
                getline(cin, login);
                send(clientSocket, login.c_str(), login.size(), 0);

                int iResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);

                if (iResult == SOCKET_ERROR) {
                    cerr << "Receive failed" << WSAGetLastError() << endl;
                }
                else {
                    string responseUsername(buffer, iResult);
                    cout << responseUsername << endl;

                    string username;
                    getline(cin, username);
                    send(clientSocket, username.c_str(), username.size(), 0);


                    isLogged = true;
                }
            }
        }
        else {

            string userCommand;
            getline(cin, userCommand);
            send(clientSocket, userCommand.c_str(), userCommand.size(), 0);

            commandHelperFunction(userCommand, clientSocket, buffer);
        }

        

        // TODO: Implement client commands and interaction with the server


    }
    
    // Clean up
    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

void commandHelperFunction(string command, SOCKET clientSocket, char* buffer) {
    if (command == "Logout") {
        cin.get();
    }
}
