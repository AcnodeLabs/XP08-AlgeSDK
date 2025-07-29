// mousepos.cpp : Defines the entry point for the application.
//
#pragma comment(lib, "ws2_32.lib")

#include <Windows.h>
#include <iostream>
#include <cstring>
#include <string>

inline void setXY(unsigned short x, unsigned short y) {
    SetCursorPos(x,y);
}

void HideMouseCursor() {
    while (ShowCursor(FALSE) >= 0); // Hide the cursor
}

void ShowMouseCursor() {
    while (ShowCursor(TRUE) < 0); // Show the cursor
}



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

unsigned short counter = 0;

unsigned short msg[3];

unsigned short prevX;
unsigned short prevY;

short page = 0;

unsigned short maxX = 1366 -1;
unsigned short maxY = 768 - 1;

int dispCount =0;

unsigned short page0x = maxX/2;
unsigned short page0y = maxY/2;

unsigned short parkX = maxX -1;
unsigned short parkY = maxY -1;


inline void park(short lx, short ly) {
    SetCursorPos(maxX-1, maxY-1);
    parkX = lx; parkY = ly;
}


inline void unpark() {
    SetCursorPos(parkX, parkY);
    parkX = maxX - 1;
    parkY = maxY - 1;
}

inline void leavingServer(short lx, short ly) {
    page0x = lx;
    page0y = ly;
    std::cout << " Leaving Server ";
    park(lx, ly);
}

inline void entringServer(short lx, short ly) {
    std::cout << " Entering Server ";
    setXY(page0x, page0y);
    unpark();
}





int post70X1(POINT p) {

    // Data to send
    msg[0] = (unsigned short)counter;
    msg[1] = (unsigned short)p.x;
    msg[2] = (unsigned short)p.y;

    if (p.x == maxX and p.x > prevX) { 
        if (page == 0) leavingServer(p.x, p.y);
        page++;
        int ph = 1;
        if (page != 0) ph = 1;
        SetCursorPos(0,ph* p.y);
        if (page == 0) ShowMouseCursor(); else HideMouseCursor();
        std::cout << "Page" << page;
      //  BlockInput(page == 1);
    }
    else if (p.x == 0 and p.x < prevX) {
        page--;
        if (page == 0) entringServer(p.x, p.y);
        int ph = 1;
        if (page != 0) ph = 1;
        SetCursorPos(maxX, ph*p.y);
        if (page == 0) ShowMouseCursor(); else HideMouseCursor();
        std::cout << "Page" << page;
     //   BlockInput(page == 1);
    } 

    std::cout << "\ncounter=" << counter << " Page#" << page << " x=" << p.x << " y=" << p.y << " dispCount=" << dispCount;

    // Send data to the server
    int bytesSent = 0;
    if (page == 1) //mac is on page1
    {
        bytesSent = sendto(sockfd_out, (const char*)msg, sizeof(unsigned short) * 3, 0, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (bytesSent != SOCKET_ERROR) counter++;
    }

  

    prevX = msg[1];
    prevY = msg[2];

    return bytesSent != SOCKET_ERROR;
}


void cleanup() {
    // Cleanup Winsock
    closesocket(sockfd_out);
    WSACleanup();
}

int main() {
    HCURSOR oldcursor = SetCursor(0);
    int ret = init_udp("192.168.137.2", 7301);
    if (ret == 0) std::cout << "Fail";
    if (ret == 1) std::cout << "Ready";
    POINT p;
    while (true) {
        p = getXY();
        if (prevX != p.x || prevY != p.y)
        {
           
             ret = post70X1(p);
            Sleep(1); // Sleep for 500 milliseconds
        }
        else {
            Sleep(1);
        }
    }
    if (ret == 0) std::cout << "Fail";
    if (ret == 1) std::cout << "Ready";
    cleanup();
}