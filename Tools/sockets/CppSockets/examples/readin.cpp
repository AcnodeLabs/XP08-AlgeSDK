/*
 * udpserver.cpp test code for UDP server sockets
 *
 * Copyright (C) 2019 Simon D. Levy
 *
 * MIT License
 */

#include <UdpServerSocket.hpp>
#include <iostream>
#include <ApplicationServices/ApplicationServices.h>

#include <string.h>
#include <unistd.h>

const short           PORT         = 7301;
static const short    BUFLEN       = 6;
static const uint32_t TIMEOUT_MSEC = 1000;


void SetMouseCursorPosition(int x, int y) {
    CGEventRef moveEvent = CGEventCreateMouseEvent(
        nullptr, kCGEventMouseMoved, CGPointMake(x, y), 
        CGMouseButton::kCGMouseButtonLeft
    );
    
    CGEventPost(kCGHIDEventTap, moveEvent);
    CFRelease(moveEvent);
}

int main()
{
    char buf[BUFLEN];

    float serverX = 1366.;
    float serverY = 768.;
    float clientX = 1920.;
    float clientY = 1200.;

    unsigned int prevCnt = 0;

    UdpServerSocket server(PORT, TIMEOUT_MSEC);

    printf("Monitoring...");

    while (true) {

        printf(">");
        fflush(stdout);

        memset(buf, 0, BUFLEN);

        *buf = 0;

        bool ret = server.receiveData(buf, BUFLEN);
        printf("<");
        if (ret) printf("-=-=-=-=-=-=-=-=-=- DATA RCVD FLAWED -=-=-=-=-=-=-=-=-");
        
        if (*buf) {
                // Interpret the two bytes as an unsigned short
     
        unsigned char c1 = buf[0];//count
        unsigned char c2 = buf[1];
             
        unsigned char b1 = buf[2];//x
        unsigned char b2 = buf[3];
        unsigned char b3 = buf[4];//y
        unsigned char b4 = buf[5];

        unsigned short cnt = ((short)c1)+((short)c2*256);
    
        unsigned short x = ((short)b1)+((short)b2*256);
        unsigned short y =  ((short)b3)+((short)b4*256);

         //   printf("Data: %d,%d\n", x,y);
        int mx = int(x* clientX/serverX);
        int my = int(y* clientY/serverY);
            printf("Count:%d Data: %X %X,%X %X     %d,%d ==> %d,%d\n", cnt, b1,b2,b3,b4, x,y, mx,my);
            if (cnt!=(prevCnt+1)) printf("!!!!!!!!!!!!!!!!!!!!MISS SOMETHING!!!!!!!!!!!!!!");
        
          //  SetMouseCursorPosition(mx,my); {
            CGEventRef moveEvent = CGEventCreateMouseEvent(
                nullptr, kCGEventMouseMoved, CGPointMake(mx, my), 
                CGMouseButton::kCGMouseButtonLeft
            );
            
            CGEventPost(kCGHIDEventTap, moveEvent);
            CFRelease(moveEvent);
            //}
            prevCnt = cnt;
        }

        else {
            printf("\n");
           // sleep(1);
        }

    }

    server.closeConnection();

    return 0;
}
