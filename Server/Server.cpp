// Server.cpp : This file contains the 'main' function. Program execution begins and ends there.

// EmailServer.cpp
#include <iostream>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define BUFFER_SIZE 1024

using namespace std;

bool isLogged = false;

void proccesReceivedCommand(string command, SOCKET serverSocket, char* buffer);

int main() {
    // Initialize WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "Failed to initialize WinSock." << endl;
        return 1;
    }

    // Create a socket
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    // Set up the server address structure
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(27015);  // Example port number
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(serverSocket, reinterpret_cast<sockaddr*>(&serverAddress), sizeof(serverAddress)) == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        cerr << "Listen failed: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Email Server listening for incoming connections..." << endl;

    // Accept connections and handle client requests
    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) {
            cerr << "Accept failed: " << WSAGetLastError() << endl;
        }
        else {
            
            char buffer[BUFFER_SIZE];

            if (isLogged == false) {
                string message = "Please login: ";
                send(clientSocket, message.c_str(), message.size(), 0);

                int iResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                if (iResult == SOCKET_ERROR) {
                    cerr << "Receive failed: " << WSAGetLastError() << endl;
                }
                else {
                    string receivedCommand(buffer, iResult);
                    if (receivedCommand == "Login") {
                        string message = "Enter username: ";
                        send(clientSocket, message.c_str(), message.size(), 0);

                        int iResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);

                        if (iResult == SOCKET_ERROR) {
                            cerr << "Receive failed" << WSAGetLastError() << endl;
                        }
                        else {
                            string responseMessage(buffer, iResult);
                            //Server dobija username
                            cout << "Ulogovao se korisnik: " << responseMessage << endl;
                            isLogged = true;
                        }
                    }
                    else {
                        string errorMessage = "Error";
                        send(clientSocket, errorMessage.c_str(), errorMessage.size(), 0);
                    }
                }       
            }
            else {
                string message = "Enter command Login/Logout/Send/Check/Stat/Delete/Clean/Receive";
                send(clientSocket, message.c_str(), message.size(), 0);

                int iResult = recv(clientSocket, buffer, BUFFER_SIZE, 0);
                if (iResult == SOCKET_ERROR) {
                    cerr << "Receive failed: " << WSAGetLastError() << endl;
                }
                else {
                    string receivedCommand(buffer, iResult);
                    cout << "Received command from client: " << receivedCommand << endl;

                    proccesReceivedCommand(receivedCommand, clientSocket, buffer);
                }
                
            }

            

            closesocket(clientSocket);
        }
    }



    // Clean up
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

void proccesReceivedCommand(string command, SOCKET clientSocket, char* buffer) {
    if (command == "Logout") {

    }
    else if (command == "Send") {

    }
    else if (command == "Check") {

    }
    else if (command == "Stat") {

    }
    else if (command == "Delete") {

    }
    else if (command == "Clean") {

    }
    else if (command == "Receive") {

    }
    else {
        cout << "Uneta nepoznata komanda pokusajte opet" << endl;
    }
}
