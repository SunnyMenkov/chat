//
// Created by amenk on 03.05.2024.
//

#include <iostream>
#include <WS2tcpip.h>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include "globals.h"
#include "server.h"

#pragma comment (lib, "ws2_32.lib")

using namespace std;


//Структура всех занятых и не занятых логинов и паролей

extern INFO users_info[1000];
extern all_logins data_inf[1000];




string user_login(string cmd, string login, string password, SOCKET sock){
        string message;
        ifstream user_in("inf_users.txt",ifstream::app);
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
                        if (users_info[k].user_socket == to_string(sock)) {
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
                            if (users_info[k].user_socket == to_string(sock))
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


void send_message(fd_set master,SOCKET listening,SOCKET sock,char buf[]){
    string sendtofile;
    string flag ="" ;
    int f = 0;
    time_t now = time(0);
    char* time = asctime(localtime(&now));
    time[strlen(time) - 1] = '\0';

    ofstream history("history.txt",ofstream::app);


    for (int i=0;i<master.fd_count;i++){
        ostringstream ss;
        SOCKET outSock = master.fd_array[i];
        if (outSock!= listening && outSock!= sock){
            for (int k = 0; k < cnt_users_inf; k++)
            {

                if (users_info[k].user_socket == to_string(sock))
                {
                  //  cout <<" we are here";
                    if (users_info[k].flag_user_name == 1) {
                         ss  << users_info[k].login << ": " << buf << "\r\n";
                    flag = users_info[k].login;
                    }
                    else ss  << users_info[k].user_socket << ": " << buf << "\r\n";

                    ss.flush();
                    break;
                }
            }
            //ss  << users[sock] << ": " << buf << "\r\n";
            //сумма проверяет на пустоту
            int sumk=0;
            for (int i=0;i<4096;i++){
                sumk+=buf[i];
            }

            string strOut = ss.str();
            if (sumk!=23) {
         //   cout << sumk <<"= sum" <<endl;
                //cout << time << " message to " << strOut << ": " << buf<<std::endl;
                send(outSock, strOut.c_str(), strOut.size() + 1, 0);

                if (flag == "")  sendtofile =  to_string(sock) + ": "  + string(buf) + "\n";
                else sendtofile = flag + ": "  + string(buf) + "\n";


                if (history.is_open() && f==0){
                    if (buf[0]!='/' && sendtofile!= to_string(sock) + ": \r\n" &&  sendtofile!= flag + ": \r\n") {
                        history << sendtofile;
                        history.flush();
                        cout << sendtofile;
                        f=1;
                    }
                }

           }
        }
    }



}

void pr_mesg(string cmd,SOCKET sock)
{
    int cnt_bfm = 9, user_flag = 0;
    while (cmd[cnt_bfm] !=' ')
    {
        cnt_bfm++;
    }
    string recepient = cmd.substr(9, cnt_bfm - 9); // recepient
    cout<<"Кол-во юзеров: "<<cnt_users_inf<<"\n";
    for (int j = 0; j < cnt_users_inf; j++)
    {
      
        if ((users_info[j].login == recepient) || (users_info[j].user_socket == recepient))
        {

            user_flag = 1;
            string text_message;
            for (int k = 0; k < cnt_users_inf; k++)
            {
                cout << users_info[k].user_socket << "USER SOCKET "<<endl;
                if (users_info[k].user_socket == to_string(sock)) //sender
                {
                    cout<< users_info[k].user_socket <<" отправил приватное смс "<<recepient<<"\n";
                    string firstuser,seconduser;

                    if (users_info[k].login!="")
                        firstuser = users_info[k].login;
                    else
                        firstuser = users_info[k].user_socket;

                    if (users_info[j].login!="")
                        seconduser = users_info[j].login;
                    else
                        seconduser = users_info[j].user_socket;

                    string arr[] = {firstuser,seconduser };
                    sort(arr, arr + 2);


                    ofstream private_history_out("private_history/"+arr[0]+" "+arr[1]+".txt",ofstream::app);
                    ifstream private_history_read("private_history/"+arr[0]+" "+arr[1]+".txt");

                    if (users_info[k].flag_user_name == 1)
                    {text_message =users_info[k].login+": "+cmd.substr(cnt_bfm+1, cmd.length()-cnt_bfm);}
                    else{
                        text_message = users_info[k].user_socket+": "+cmd.substr(cnt_bfm+1, cmd.length()-cnt_bfm);
                    }
                    private_history_out<<text_message<<std::endl;



                    text_message = "private from "+text_message+"\r\n";

                    send(stoi(users_info[j].user_socket), text_message.c_str(), text_message.size()+1, 0);

                    string line,prev_private_history;
                    while(getline(private_history_read, line))
                    {
                        //std::cout << "line:" << line << std::endl;
                        prev_private_history +='\n'+ line;
                    }

                    send(stoi(users_info[k].user_socket), prev_private_history.c_str(),prev_private_history.size()+1,0);

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
        string message = "Пользователя с данным никнеймом не существует.\r\n";
        send(sock, message.c_str(),message.size()+1,0);
    }
}

int server()
{

// чтобы нам с тобой не менять каждый раз путь до файла, я стёр абсолютный путь
// теперь оно всё сохраняется по дефолту силиона
// это не папка проекта, а внутренняя папка cmake-build-debug
//то есть ищи эти файлы там (cmake-build-debug/inf_users.txt) например
//таким образом нам не придётся каждый раз это менять

    ifstream user_in("inf_users.txt",ifstream::app);
    ofstream history_out("history.txt",ofstream::app);
    ofstream users_out("inf_users.txt",ifstream::app);


//напоминаю, что ifstream -- для чтения инпута из файла
// ofstream -- для записи вывода в файл

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
    listening = socket(AF_INET, SOCK_STREAM, 0);
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
    master;
    FD_ZERO(&master);
    FD_SET(listening, &master);
//    int count=0;
    while(true){


        fd_set copy = master;

        int socketCount = select(0, &copy, nullptr, nullptr, nullptr);

        for (int i=0;i<socketCount;i++){
            ostringstream ss;
            int prop = 0;
            SOCKET sock = copy.fd_array[i];

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
                cout <<"Новый сокет подключился!: " <<client <<'\n';


                // welcome message
                string welcomeMsg = "\r\nУспешное подключение к серверу.\r\n"
                                    "Добро пожаловать! Введите /command, чтобы увидеть список доступных команд\r\n";
                string prev_history="";
                ifstream prev_out("history.txt",ifstream::app);

                string line;
                while(getline(prev_out, line))
                {
                    //std::cout << "line:" << line << std::endl;
                    if (line!="\r\n"  && line.length()>0)
                    prev_history +='\n'+ line;
                }
                send(client, prev_history.c_str(),prev_history.size()+1,0);
                send(client, welcomeMsg.c_str(),welcomeMsg.size()+1,0);
//                history_out << "\nhistory from " << asctime(localtime(&date));
//                history_out.flush();

            }
            else {
                char buf[4096];
                ZeroMemory(buf, 4096); //буфер

                // принять новое смс
                int bytesIn = recv(sock,buf,4096,0);


//                if (history_out.is_open()){
//                    int sumk=0;
//                    for (int i=0;i<4096;i++){
//                        sumk+=buf[i];
//                    }
//
//                    if (sumk!=23 && buf[0]!='/') {
//                        history_out << sock << ": " << buf<<std::endl;
//                        history_out.flush();
//                    }
//
//                }
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
                        if (cmd.substr(0,8) == "/pr_mesg") pr_mesg(cmd,sock);
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
                            string login_register = cmd.substr(10, index_reg - 10), password_register = cmd.substr(index_reg+1, cmd.length() - index_reg-3);
                            cout << "\nlogin len " << login_register.length()<< ": "<< login_register<<endl;

                            //password_register.erase(std::remove_if(password_register.begin(), password_register.end(), [](char c) { return c == '\n'; }), password_register.end());

                            cout << "pass len " << password_register.length() << ": " << password_register<<endl;

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
                                user_in.seekg(0,ios_base::beg);
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
                                    users_out.flush();
                                }
                            }

                        }

                        if (cmd.substr(0,6) == "/login") user_login(cmd,"","",sock);

                        // Unknown command
                        continue;
                    }
                    //отправляем смс другим клиентам, проверяем что не на листенинг сокет
                    send_message(master,listening,sock,buf);

//                    for (int i=0;i<master.fd_count;i++){
//
//                        SOCKET outSock = master.fd_array[i];
//                        if (outSock!= listening && outSock!= sock){
//                            for (int k = 0; k < cnt_users_inf; k++)
//                            {
//                                if (users_info[k].user_socket == to_string(sock))
//                                {
//                                    if (users_info[k].flag_user_name == 1)
//                                        ss  << users_info[k].login << ": " << buf << "\r\n";
//                                    else ss  << users_info[k].user_socket << ": " << buf << "\r\n";
//                                    break;
//                                }
//                            }
//
//
//                            //ss  << users[sock] << ": " << buf << "\r\n";
//
//                            int sumk=0;
//                            for (int i=0;i<4096;i++){
//                                sumk+=buf[i];
//                            }
//                            string strOut = ss.str();
//                            cout << outSock << ": " << buf  << "len:" <<sumk<<'\n';
//
////                            if (strlen(buf)!=2 && buf!="\r\n") {
//                            if (sumk!=23) {
//                                send(outSock, strOut.c_str(), strOut.size() + 1, 0);
//                            }
//
//
//                        }
//
//                    }

                }


            }

        }

    }
    FD_CLR(listening, &master);
    closesocket(listening);

    //Cleanup winsock
    WSACleanup();

    // закрываем файлики
    history_out.close();
    users_out.close();
    user_in.close();

    system("pause");

    // Add our first socket that we're interested in interacting with; the listening socket!
    // It's important that this socket is added for our server or else we won't 'hear' incoming
    // connections
    return 0;

}