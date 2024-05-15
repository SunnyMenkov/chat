//
// Created by Alexander on 15.05.2024.
//
#include <WS2tcpip.h>
#include "globals.h"
//
//void globals() {
//// Initialze winsock
//    WSADATA wsData;
//    WORD ver = MAKEWORD(2, 2);
//
//    int wsOk = WSAStartup(ver, &wsData);
//    if (wsOk != 0) {
//        //cerr << "Can't Initialize winsock! Quitting" << '\n';
//
//    }
//
//// Create a socket
//    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
//    if (listening == INVALID_SOCKET) {
//        //cerr << "Can't create a socket! Quitting" << '\n';
//
//    }
//
//// Bind the ip address and port to a socket
//    sockaddr_in hint;
//    hint.sin_family = AF_INET;
//    hint.sin_port = htons(54000);
//    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ....
//
//    bind(listening, (sockaddr *) &hint, sizeof(hint));
//
//// Tell Winsock the socket is for listening
//    listen(listening, SOMAXCONN);
//
//// fd_set - структура сокета в С++
//    fd_set master;
//    FD_ZERO(&master);
//    FD_SET(listening, &master);
//    fd_set copy = master;
//    SOCKET sock = copy.fd_array[0];
//
//
//}




fd_set master;
SOCKET sock;
SOCKET listening;

