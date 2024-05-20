#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include "globals.h"

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int client()
{
    string ipAddress = "127.0.0.1";			// IP Address of the server
    int port = 54000;						// Listening port # on the server

    // Initialize WinSock
    WSAData data;
    WORD ver = MAKEWORD(2, 2);
    int wsResult = WSAStartup(ver, &data);
    if (wsResult != 0)
    {
        cerr << "Can't start Winsock, Err #" << wsResult << endl;
        return 0;
    }

    // Create socket
//    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    GUIsock = socket(AF_INET, SOCK_STREAM, 0);
    if (GUIsock == INVALID_SOCKET)
    {
        cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
        WSACleanup();
        return 0;
    }

    // Fill in a hint structure
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(port);
    inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

    //FD_SET(GUIsock, &master); // самая важная часть, добавить сокет в листенинг

    // Connect to server
    int connResult = connect(GUIsock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(GUIsock);
        WSACleanup();
        return 0;
    }

//
//
//    // Do-while loop to send and receive data
//    char buf[4096];
//    string userInput;
//
//    do
//    {
//        // Prompt the user for some text
//        cout << "> ";
//        getline(cin, userInput);
//
//        if (userInput.size() > 0)		// Make sure the user has typed in something
//        {
//            // Send the text
//            int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
//            if (sendResult != SOCKET_ERROR)
//            {
//                // Wait for response
//                ZeroMemory(buf, 4096);
//                int bytesReceived = recv(sock, buf, 4096, 0);
//                if (bytesReceived > 0)
//                {
//                    // Echo response to console
//                    cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
//                }
//            }
//        }
//
//    } while (userInput.size() > 0);

    // Gracefully close down everything
   // closesocket(sock);
    WSACleanup();
}