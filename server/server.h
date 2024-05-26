//
// Created by Alexander on 15.05.2024.
//
#include <WS2tcpip.h>
#include <string>
using namespace std;
#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H


extern fd_set master;
string get_connected_sockets();
int server();
string user_login(string cmd, string login, string password,SOCKET sock);
//void send_gui(SOCKET sock,string message);
void send_message(SOCKET sock,char buf[]);
void pr_mesg(string cmd, SOCKET sock);

#endif //CHAT_SERVER_H
