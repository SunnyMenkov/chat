//
// Created by Alexander on 15.05.2024.
//
#include <WS2tcpip.h>

#ifndef CHAT_GLOBALS_H
#define CHAT_GLOBALS_H

extern char text[1024];
extern fd_set master;
extern SOCKET listening,GUIsock,GUIsock2;
extern int cnt_users_inf;
extern int cnt_data_info;


#endif //CHAT_GLOBALS_H
