//
// Created by amenk on 03.05.2024.
//

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>

#pragma comment (lib, "ws2_32.lib")

using namespace std;

int main()
{
    time_t date = time(nullptr);

    ofstream out;
    out.open("C:\\rep\\chat\\history.txt",ofstream::app);
    if (out.is_open())
    {
        out << "history from " << asctime(localtime(&date)) << std::endl;
    }

    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        cerr << "Can't Initialize winsock! Quitting" << endl;
        return 0;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        return 0;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY; // Could also use inet_pton ....

    bind(listening, (sockaddr*)&hint, sizeof(hint));

    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    // fd_set - структура сокета в С++
    fd_set master;
    FD_ZERO(&master);
    FD_SET(listening, &master);

    string users[999];
    while(true){

        fd_set copy = master;

        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        for (int i=0;i<socketCount;i++){
            SOCKET sock = copy.fd_array[i];

            if (users[sock]=="") users[sock] = to_string(sock);
            if (sock == listening){
                // принять соединение
                SOCKET client = accept(listening, nullptr, nullptr);
                //users[client] = to_string(client);

                // add the new connection to the list of connected clients
                FD_SET(client, &master); // самая важная часть, добавить сокет в листенинг
                cout <<"!: " <<client <<endl;



                // welcome message
                string welcomeMsg = "Успешное подключение к серверу.\r\n";
                send(client, welcomeMsg.c_str(),welcomeMsg.size()+1,0);
            }
            else {
                char buf[4096];
                ZeroMemory(buf, 4096); //буфер

                // принять новое смс
                int bytesIn = recv(sock,buf,4096,0);
                if (out.is_open()){
                    out <<sock <<": " << buf<<std::endl;
                }


                if (bytesIn <= 0){
                    //drop the client
                    closesocket(sock);
                    FD_CLR(sock, &master);

                }
                else{
                    //команды
                    if (buf[0] == '/')
                    {
                        // Is the command quit?
                        string cmd = string(buf, bytesIn);
                        if (cmd.substr(0,5) == "/name")
                        {
                          //
                          users[sock] = cmd.substr(6,cmd.length()-6);
                        }

                        // Unknown command
                        continue;
                    }

                    //отправляем смс другим клиентам, проверяем что не на листенинг сокет
                    for (int i=0;i<master.fd_count;i++){
                        SOCKET outSock = master.fd_array[i];
                        if (outSock!= listening && outSock!= sock){

                            ostringstream ss;
                            ss  << users[sock] << ":" << buf << "\r\n";
                            string strOut = ss.str();


                            send(outSock, strOut.c_str(), strOut.size()+1, 0);
                            cout << outSock << ": " << buf << endl;
                        }
                    }

                }


            }

        }


    }
    FD_CLR(listening, &master);
    closesocket(listening);

    // Cleanup winsock
    WSACleanup();

    system("pause");
    out.close();
    // Add our first socket that we're interested in interacting with; the listening socket!
    // It's important that this socket is added for our server or else we won't 'hear' incoming
    // connections





}