//
// Created by Alexander on 15.05.2024.
//
#include "C:\Users\Alexander\Documents\GitHub\chat\server\globals.h"
#include <thread>
#include "clientgui.h"
#include "client.h"

using namespace std;

int main(){
    thread gui(maingui);
    cnt_users_inf=16;
    client();
    cout << clientsock<< " your socket"<<endl;

    cout<<"Multithreading started"<<endl;


    gui.join();
    return 0;
}