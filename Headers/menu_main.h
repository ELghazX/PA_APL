#ifndef MENU_MAIN_H
#define MENU_MAIN_H

#include <conio.h>
#include "UIUIX.h"
#include "akun.h"
#include <chrono>
#include <iostream>
#include <thread>
using namespace std;
using namespace chrono;
using namespace this_thread;
void Login(int &gagal);
void Regis();
int gagal = 0;

void Menu(){
    system("cls");
    if (gagal >= 3){
        cout << "Anda sudah mencoba login lebih dari 3 kali" << endl;

        for (int i = 15; i > 0; i--) {
            cout << "Silahkan coba lagi dalam " << i << " detik";
            sleep_for(seconds(1));
            cout << "\r";
        }
        gagal = 0;
        Menu();
    }
    string opsi[3] = {"Login", "Register", "Exit"};
    string opsi_header = "MENU UTAMA";

    int pilih = showmenu(3, opsi, opsi_header);

    if (pilih == 0){
        Login(gagal);
    }
    else if (pilih == 1){
        Regis();
    }
    else{
        exit(0);
    }

    getch();
    Menu();
}

#endif
