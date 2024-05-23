//
// Created by Alexander on 15.05.2024.
//
#include <WS2tcpip.h>
#include <string>
#include "globals.h"

using namespace std;

fd_set master;
SOCKET sock;
SOCKET GUIsock=0;
SOCKET listening;
