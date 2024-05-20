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

    thread t2(maingui);
    thread t1(server);
    thread t3(client);

  thread t4(client);
//    thread t6(client);




    //  thread t4(client);


    cout<<"Multithreading started";

    t2.join();

    return 0;
}