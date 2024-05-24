//
// Created by Alexander on 15.05.2024.
//
//#include <WS2tcpip.h>
#include "globals.h"


INFO users_info[1000];
all_logins data_inf[1000];
fd_set master;
SOCKET GUIsock;
SOCKET GUIsock2;
SOCKET listening;
int cnt_users_inf=2;
int cnt_data_info=0;

