//
// Created by Alexander on 15.05.2024.
//
#include <thread>
#include "clientgui.h"
#include "server.h"
#include "client.h"
#include "globals.h"

using namespace std;

int main(){

    thread gui(maingui);
    thread s(server);
    client();
    client();



    cout<<"Multithreading started"<<endl;

    gui.join();

    return 0;
}