//
// Created by Alexander on 15.05.2024.
//
#include <iostream>
#include <thread>
#include "server.h"
#include "globals.h"


using namespace std;

int main(){
    thread s(server);
    cout<<"Multithreading started"<<endl;
    users_info[0].login = "(server)";
    s.join();

    return 0;
}