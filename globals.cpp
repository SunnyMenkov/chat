//
// Created by Alexander on 15.05.2024.
//
#include <WS2tcpip.h>
#include "globals.h"


fd_set master;
SOCKET GUIsock;
SOCKET GUIsock2;
SOCKET listening;
int cnt_users_inf=2, cnt_data_info=0;

