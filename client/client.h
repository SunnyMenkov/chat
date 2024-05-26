//
// Created by Alexander on 20.05.2024.
//
#include <iostream>
#include <string>
#include <WS2tcpip.h>

#ifndef UNTITLED_CLIENT_H
#define UNTITLED_CLIENT_H

void send_message(SOCKET sock,char buf[]);
string user_login(string cmd, string login, string password, SOCKET sock);
void send_gui(SOCKET sock,string message);
int client();
extern SOCKET clientsock,globalClientSock;
#endif //UNTITLED_CLIENT_H
