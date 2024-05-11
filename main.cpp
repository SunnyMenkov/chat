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
int cnt_users_inf = 0, cnt_data_info = 0;

//Структура всех занятых и не занятых логинов и паролей
struct all_logins
        {
    string login, password;
        };
//Структура информации каждого сокета
struct INFO
        {
    string login, password, user_socket;
    int flag_user_name = 0;
        };

INFO users_info[1000];
all_logins data_inf[1000];

int main()
{
    time_t date = time(nullptr);
// чтобы нам с тобой не менять каждый раз путь до файла, я стёр абсолютный путь
// теперь оно всё сохраняется по дефолту силиона
// это не папка проекта, а внутренняя папка cmake-build-debug
//то есть ищи эти файлы там (cmake-build-debug/inf_users.txt) например
//таким образом нам не придётся каждый раз это менять

    ofstream out, users_out;
    users_out.open("inf_users.txt",ofstream::app);
    out.open("history.txt",ofstream::app);
    ifstream user_in("inf_users.txt",ifstream::app);

    if (out.is_open())
    {
        cout << "working file"<<endl;
        out << "history from " << asctime(localtime(&date)) << std::endl;
    }


    // Initialze winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        cerr << "Can't Initialize winsock! Quitting" << '\n';
        return 0;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Can't create a socket! Quitting" << '\n';
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

    while(true){

        fd_set copy = master;

        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        for (int i=0;i<socketCount;i++){
            int prop = 0;
            SOCKET sock = copy.fd_array[i];
            ostringstream ss;
            for (int l = 0; l < cnt_users_inf; l++)
            {
                if (users_info[l].user_socket == to_string(sock))
                {
                    prop = 1;
                    break;
                }
            }
            if (prop == 0)
            {
                users_info[cnt_users_inf].user_socket = to_string(sock);
                cout<<cnt_users_inf<<"-----"<<users_info[cnt_users_inf].user_socket<<"\n";
                cnt_users_inf++;
            }
            if (sock == listening){
                // принять соединение
                SOCKET client = accept(listening, nullptr, nullptr);
                //users[client] = to_string(client);

                // add the new connection to the list of connected clients
                FD_SET(client, &master); // самая важная часть, добавить сокет в листенинг
                cout <<"!: " <<client <<'\n';



                // welcome message
                string welcomeMsg = "Успешное подключение к серверу.\r\n"
                                    "Добро пожаловать! Введите /command, чтобы увидеть список доступных команд\r\n";
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

                        string cmd = string(buf, bytesIn), from_file;
//                        if (cmd.substr(0,5) == "/name")
//                        {
//                          users[sock] = cmd.substr(6,cmd.length()-6);
//                        }
                        if (cmd.substr(0,8) == "/pr_mesg")
                        {
                            int cnt_bfm = 9, user_flag = 0;
                            while (cmd[cnt_bfm] !=' ')
                            {
                                cnt_bfm++;
                            }
                            string user_name = cmd.substr(9, cnt_bfm - 9);
                            cout<<cnt_users_inf<<"\n";
                            for (int j = 0; j < cnt_users_inf; j++)
                            {
                                cout<<users_info[j].user_socket<<" "<<user_name<<" "<<(users_info[j].user_socket == user_name)<<"\n";
                                if ((users_info[j].login == user_name) || (users_info[j].user_socket == user_name))
                                {
                                    user_flag = 1;
                                    string text_message;
                                    for (int k = 0; k < cnt_users_inf; k++)
                                    {
                                        if (users_info[k].user_socket == to_string(sock))
                                        {
                                            if (users_info[k].flag_user_name == 1) text_message = "private from " + users_info[k].login+": "+cmd.substr(cnt_bfm+1, cmd.length()-cnt_bfm);
                                            else text_message = "private from " + users_info[k].user_socket+": "+cmd.substr(cnt_bfm+1, cmd.length()-cnt_bfm);
                                            send(stoi(users_info[j].user_socket), text_message.c_str(), text_message.size()+1, 0);
                                            break;
                                        }
                                    }
//                                    if (users_info[j].flag_user_name == 1) text_message = "private from " + users_info[j].login+": "+cmd.substr(cnt_bfm+1, cmd.length()-cnt_bfm);
//                                    else text_message = "private from " + users_info[j].user_socket+": "+cmd.substr(cnt_bfm+1, cmd.length()-cnt_bfm);
//                                    send(stoi(users_info[j].user_socket), text_message.c_str(), text_message.size()+1, 0);
                                    //break;
                                }
                            }
                            if (user_flag == 0)
                            {
                                string message = "Пользователя с данным никнеймом не существует не существует.\r\n";
                                send(sock, message.c_str(),message.size()+1,0);
                            }
                        }
                        if (cmd.substr(0,8) == "/command")
                        {
                            string message = "\r\nСписок комманд:\r\n"
                                             "/login <<login>> <<password>> - для входа в существующий аккаунт пользователя.\r\n"
                                             "/register <<login>> <<password>> - для регистрации аккаунта в системе.\r\n"
                                             "/pr_mesg <<login>> <<message>> - личное сообщение пользователю.\r\n"
                                             "\r\n";
                            send(sock, message.c_str(),message.size()+1,0);
                        }
                        if (cmd.substr(0,9) == "/register")
                        {

                            int index_reg = 10, flag_reg = 0;
                            while (cmd[index_reg] != ' ')
                            {
                                index_reg++;
                            }
                            string login_register = cmd.substr(10, index_reg - 10), password_register = cmd.substr(index_reg+1, cmd.length() - index_reg);

                            for (int i = 0; i < cnt_data_info;i++)
                            {
                                    if (login_register == data_inf[i].login) {
                                        flag_reg = 1;
                                        //ss << "Такой логин уже существует.\r\n";
                                        string message = "Такой логин уже существует.\r\n";
                                        send(sock, message.c_str(), message.size() + 1, 0);
                                        break;
                                    }
                            }

                            if (flag_reg == 0 ) {
                                while (getline(user_in, from_file)) {
                                    int index_f = 0;
                                    while (from_file[index_f] != ' ') {
                                        index_f++;
                                    }
                                    string user_login_file = from_file.substr(0, index_f), user_password_file = from_file.substr( index_f + 1, from_file.length() - index_f);
                                    if (user_login_file == login_register) {
                                        flag_reg = 1;
                                        //ss << "Такой логин уже существует.\r\n";
                                        string message = "Такой логин уже существует.\r\n";
                                        send(sock, message.c_str(),message.size()+1,0);
                                        break;
                                    }
                                }
                                //This login is free
                                if (flag_reg == 0)
                                {
                                    for (int k = 0; k < cnt_users_inf; k++) {
                                        cout<<users_info[k].user_socket<<" "<< k <<" "<<to_string(sock)<<" "<< (users_info[k].user_socket == to_string(sock))<<"\n";
                                        if (users_info[k].user_socket == to_string(sock)) {
                                            users_info[k].login = login_register;
                                            users_info[k].password = password_register;
                                            users_info[k].flag_user_name = 1;
                                            break;
                                        }
                                    }
                                    data_inf[cnt_data_info].login = login_register;
                                    data_inf[cnt_data_info].password = password_register;
                                    cnt_data_info++;
                                    cout<<"password_register = "<<password_register;
                                    string message = "Добро пожаловать, "+ login_register+ ".\r\n";
                                    send(sock, message.c_str(),message.size()+1,0);
                                    users_out << login_register<<" "<<password_register<<std::endl;
                                }
                            }

                        }
                        if (cmd.substr(0,6) == "/login")
                        {
                            int index_p = 7, flag_log = 0;
                            while (cmd[index_p] != ' ')
                            {
                                index_p++;
                            }
                            string user_login = cmd.substr(7, index_p - 7), user_password = cmd.substr(index_p+1, cmd.length() - index_p);
                            cout<<user_password;
                            //Check now
                            for (int i = 0; i < cnt_data_info;i++)
                            {
                                if (user_login == data_inf[i].login) {
                                    flag_log = 1;
                                    if (user_password == data_inf[i].password) {
                                        for (int k = 0; k < cnt_users_inf; k++) {
                                            if (users_info[k].user_socket == to_string(sock)) {
                                                users_info[k].login = user_login;
                                                users_info[k].password = user_password;
                                                users_info[k].flag_user_name = 1;
                                                break;
                                            }
                                        }
                                        string message = "Успешная авторизация.\r\n";
                                        send(sock, message.c_str(), message.size() + 1, 0);
                                    }
                                    else {
                                        //ss<<"Неправильно введен пароль.\r\n";
                                        string message = "Неправильно введен пароль.\r\n";
                                        send(sock, message.c_str(), message.size() + 1, 0);
                                    }
                                    break;
                                }

                            }
                            if (flag_log == 0) {
                                while (getline(user_in, from_file)) {
                                    int index_f = 0;
                                    while (from_file[index_f] != ' ') {
                                        index_f++;
                                    }
                                    string user_login_file = from_file.substr(0,
                                                                              index_f), user_password_file = from_file.substr(
                                            index_f + 1, from_file.length() - index_f);
                                    if (user_login_file == user_login) {
                                        flag_log = 1;
                                        if (user_password_file == user_password) {
                                            for (int k = 0; k < cnt_users_inf; k++)
                                            {
                                                if (users_info[k].user_socket == to_string(sock))
                                                {
                                                    users_info[k].login = user_login;
                                                    users_info[k].password = user_password;
                                                    users_info[k].flag_user_name = 1;
                                                    break;
                                                }
                                            }
                                        } else {
                                            //ss << "Неправильно введен логин или пароль.\r\n";
                                            string message = "Неправильно введен логин или пароль.\r\n";
                                            send(sock, message.c_str(),message.size()+1,0);
                                        }
                                        break;
                                    }
                                }
                                if (flag_log == 0)
                                {
                                    //ss<<"Такого логина не существует. \r\n";
                                    string message = "Такого логина не существует.\r\n";
                                    send(sock, message.c_str(),message.size()+1,0);
                                }
                            }
                            //Check in history


                        }

                        // Unknown command
                        continue;
                    }
                    //отправляем смс другим клиентам, проверяем что не на листенинг сокет
                    for (int i=0;i<master.fd_count;i++){

                        SOCKET outSock = master.fd_array[i];
                        if (outSock!= listening && outSock!= sock){
                            for (int k = 0; k < cnt_users_inf; k++)
                            {
                                if (users_info[k].user_socket == to_string(sock))
                                {
                                    if (users_info[k].flag_user_name == 1) ss  << users_info[k].login << ": " << buf << "\r\n";
                                    else ss  << users_info[k].user_socket << ": " << buf << "\r\n";
                                    break;
                                }
                            }


                            //ss  << users[sock] << ": " << buf << "\r\n";

                            int sumk=0;
                            for (int i=0;i<4096;i++){
                                sumk+=buf[i];
                            }
                            string strOut = ss.str();
                            cout << outSock << ": " << buf  << "len:" <<sumk<<'\n';

//                            if (strlen(buf)!=2 && buf!="\r\n") {
                            if (sumk!=23) {
                                send(outSock, strOut.c_str(), strOut.size() + 1, 0);
                            }


                        }

                    }

                }


            }

        }

    }
    FD_CLR(listening, &master);
    closesocket(listening);



    //Cleanup winsock
    WSACleanup();

    // закрываем файлики
    out.close();
    users_out.close();
    user_in.close();

    system("pause");

    // Add our first socket that we're interested in interacting with; the listening socket!
    // It's important that this socket is added for our server or else we won't 'hear' incoming
    // connections





}