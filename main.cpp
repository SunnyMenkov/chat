//
// Created by Alexander on 15.05.2024.
//
#include <thread>
#include "clientgui.h"
#include "server.h"
#include "globals.h"

using namespace std;

int main(){
    //globals();
    //maingui();
    thread t2(maingui);
    thread t1(server);


    cout<<"asDFPOASDFL;SDFLPSDL;F";
    cout<<sock;

    t2.join();



//    char buf[4096];
//    strcpy(buf,text);


    if (ImGui::Button("Button")) {cout << "sent!"; }

            return 0;
}