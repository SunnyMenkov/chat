//
// Created by Alexander on 15.05.2024.
//
#include <WS2tcpip.h>



#ifndef CHAT_GLOBALS_H
#define CHAT_GLOBALS_H
using namespace std;
extern char text[1024];
extern fd_set master;
extern SOCKET listening,sock,GUIsock;


#endif //CHAT_GLOBALS_H
