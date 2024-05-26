#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <fstream>
#include "C:\Users\Alexander\Documents\GitHub\chat\server\globals.h"
#include "client.h"
//#include "C:\Users\Alexander\Documents\GitHub\chat\server\server.h"
#include "sstream"

#pragma comment(lib, "ws2_32.lib")
using namespace std;

string user_login(string cmd, string login, string password, SOCKET sock){
    string message;
    ifstream user_in("C:\\Users\\Alexander\\Documents\\GitHub\\chat\\server\\cmake-build-debug\\inf_users.txt",ifstream::app);
    string from_file;
    int index_p = 7, flag_log = 0;
    while (cmd[index_p] != ' ')
    {
        index_p++;
    }

    if (!cmd.empty()) {
        login = cmd.substr(7, index_p - 7);
        password = cmd.substr(index_p + 1, cmd.length() - index_p - 3);
        cout <<"данные пользователя: "<< login << " " << password<<endl;
    }

    //cout<<password;
    //Check now
    for (int i = 0; i < cnt_users_inf;i++)
    {
        if (login == data_inf[i].login) {
            flag_log = 1;
            cout<<password<<" "<<data_inf[i].password;
            if (password == data_inf[i].password) {
                for (int k = 0; k < cnt_users_inf; k++) {
                    if (users_info[k].global_socket == to_string(sock)) {
                        users_info[k].login = login;
                        users_info[k].password = password;
                        users_info[k].flag_user_name = 1;
                        break;
                    }
                }
                message = "Успешная авторизация.\r\n";
                send(sock, message.c_str(), message.size() + 1, 0);
            }
            else {
                //ss<<"Неправильно введен пароль.\r\n";
                message = "Неправильно введен пароль.\r\n";
                send(sock, message.c_str(), message.size() + 1, 0);
            }
            break;
        }
    }
    if (flag_log == 0) {
        user_in.seekg(0,ios_base::beg);
        while (getline(user_in, from_file)) {

            int index_f = 0;
            while (from_file[index_f] != ' ') {
                index_f++;
            }
            string user_login_file = from_file.substr(0,
                                                      index_f), user_password_file = from_file.substr(
                    index_f + 1, from_file.length() - index_f - 1);
            if (user_login_file == login) {
                flag_log = 1;
                if (user_password_file == password) {
                    for (int k = 0; k < cnt_users_inf; k++)
                    {
                        if (users_info[k].global_socket == to_string(sock))
                        {
                            users_info[k].login = login;
                            users_info[k].password = password;
                            users_info[k].flag_user_name = 1;
                        }
                        message = "Успешная авторизация.\r\n";
                        send(sock, message.c_str(), message.size() + 1, 0);
                    }
                } else {
                    cout<<user_password_file<<"1";

                    message = "Неправильно введен логин или пароль.\r\n";
                    send(sock, message.c_str(),message.size()+1,0);
                }
                break;
            }
        }
        if (flag_log == 0)
        {

            message = "Такого логина не существует.\r\n";
            send(sock, message.c_str(),message.size()+1,0);
        }
    }

    return message;
}

void send_gui(SOCKET sock,string message){
    send(sock, message.c_str(), message.size() + 1, 0);
    cout << message<<" sent from gui";
}

using namespace std;
SOCKET clientsock = socket(AF_INET, SOCK_STREAM, 0);
SOCKET globalClientSock=0;
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

    clientsock = socket(AF_INET, SOCK_STREAM, 0);
    if (clientsock == INVALID_SOCKET)
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

    //FD_SET(clientsock, &master); // самая важная часть, добавить сокет в листенинг

    // Connect to server
    int connResult = connect(clientsock, (sockaddr*)&hint, sizeof(hint));
    if (connResult == SOCKET_ERROR)
    {
        cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
        closesocket(clientsock);
        WSACleanup();
        return 0;
    }

    char buffer[1024];

    cout << "Your LOCAL socket is: " << clientsock<<endl;
    while (true) {

        int bytes_received = recv(clientsock, buffer, sizeof(buffer), 0);
        if (bytes_received > 0) {
            std::string message(buffer, bytes_received);
            if (globalClientSock == 0 && stoi(buffer)) {globalClientSock = stoi(buffer); cout << "Your LOCAL socket is: "<<to_string(globalClientSock)<<endl;}
            // Обработка сообщения
            cout << message<<endl;

//            if (message.find("Number of clients: ") == 0) {
//                std::cout << "Server reports: " << message << std::endl;
//            }
        }

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
    return 0;
}