//
// Created by Alexander on 15.05.2024.
//
#include <WS2tcpip.h>
#include <string>
using namespace std;
#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H




int server();
string user_login(string cmd, string login, string password,SOCKET sock);
void send_message(fd_set master,SOCKET listening,SOCKET sock,char buf[]);
void pr_mesg(string cmd, SOCKET sock);

#endif //CHAT_SERVER_H
