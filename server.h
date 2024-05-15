//
// Created by Alexander on 15.05.2024.
//
#include <WS2tcpip.h>
#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H


//

int server();
void send_message(fd_set master,SOCKET listening,SOCKET sock,char buf[]);
#endif //CHAT_SERVER_H
