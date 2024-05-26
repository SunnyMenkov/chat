//
// Created by Alexander on 15.05.2024.
//
#include <WS2tcpip.h>
#include <string>
using namespace std;
#ifndef CHAT_GLOBALS_H
#define CHAT_GLOBALS_H

struct all_logins
{
    string login, password;
};

//Структура информации каждого сокета
struct INFO
{
    //Сокет, который записан в сервере и сокет, который получает клиент имеют разные номера.
    string login, password, global_socket,local_socket;
    int flag_user_name = 0;
};

extern INFO users_info[1000];
extern all_logins data_inf[1000];

extern char text[1024];
extern SOCKET listening;
extern int cnt_users_inf;
extern int cnt_data_info;



#endif //CHAT_GLOBALS_H
