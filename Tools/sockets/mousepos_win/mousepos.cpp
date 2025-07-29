// mousepos.cpp : Defines the entry point for the application.
//
#pragma comment(lib, "ws2_32.lib")

#include "mousepos.h"

#include <Windows.h>
#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <string>

POINT getXY() {
    POINT cursorPos;
    GetCursorPos(&cursorPos);
    return cursorPos;
}

WSADATA wsaData;
SOCKET sockfd_out;
sockaddr_in serverAddr;

int init_udp(std::string ipto, int port) {
    // Initialize Winsock
    
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Failed to initialize Winsock" << std::endl;
        return 0;
    }

    // Create a UDP socket
    sockfd_out = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd_out == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return 0;
    }


    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port); // Replace with your desired port number
    serverAddr.sin_addr.s_addr = inet_addr(ipto.c_str()); // Replace with the IP address of the target machine
    return 1;
}

int post70X1() {
    
    // Data to send
    const char* message = "Hello, UDP!"; // Replace with your byte data

    // Send data to the server
    int bytesSent = sendto(sockfd_out, message, strlen(message), 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Error sending data: " << WSAGetLastError() << std::endl;
        closesocket(sockfd_out);
        WSACleanup();
        return 1;
    }

    std::cout << "Sent " << bytesSent << " bytes to the server." << std::endl;

    // Cleanup Winsock
    closesocket(sockfd_out);
    WSACleanup();

    return 0;
}




int main() {
    int ret = init_udp("192.168.100.15",7301);
    if (ret == 0) std::cout << "Fail";
    if (ret == 1) std::cout << "Ready";
}