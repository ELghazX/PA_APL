#ifndef AKUN_H
#define AKUN_H

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <sstream>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include "UIUIX.h"
// #include "admin.h"

using namespace filesystem;
using namespace std;
using namespace chrono;

int pilihan, indexData = 0;
int index_akun = -1;
string inputUsername, inputPassword;
bool isLogin = false;

struct akun{
    string username;
    string password;
    string role;
    int saldo;
};

void Menu();
void MenuPelanggan();
void MenuAdmin();
// ############################################
bool isNumber(const std::string& str) {
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
        return !std::isspace(ch);
    }));
}
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}
// ############################################

akun users[64];
bool syaratpassword(string input){
    if (input.length() < 6){
        return false;
    };
    bool kapital = false;
    bool kecil = false;
    bool angka = false;
    for (char huruf : input){
        if (isupper(huruf)){
            kapital = true;
        }
        else if (islower(huruf)){
            kecil = true;
        }
        else if (isdigit(huruf)) {
            angka = true;
        }

        if (kapital && kecil && angka){
            break;
        }
    }
    return kapital && kecil && angka;
}

void ReadFile(){
    ifstream database("Database/Data_User.csv");
    if (database.is_open()){
        string baris;
        getline(database, baris);
        indexData = 0;

        while (getline(database, baris)){
            stringstream ss(baris);
            string field;
            int fieldIndex = 0;

            akun user;

            while (getline(ss, field, ',')){
                switch (fieldIndex){
                case 0:
                     user.username = field;

                    break;
                case 1:
                    user.password = field;

                    break;
                case 2:
                    user.role = field;

                    break;
                case 3:
                    user.saldo = stoi(field);

                    break;
                }
                fieldIndex++;
            }

            users[indexData++] = user;
        }

        database.close();
    }
}
void input(){
    string regis_header = "REGISTRASI";
    string regis[2] = {"Registrasi", "Kembali"};
    int pilih = showmenu(2, regis, regis_header);
    if(pilih == 1){
        Menu();
    }
    system("cls");

    cout << "REGISTRASI\n" << endl;

    cout << "Masukkan Username : ";
    getline(cin, users[indexData].username );
    trim(users[indexData].username);
    if (users[indexData].username.empty()){
        cout << "Username tidak boleh kosong" << endl;
        getch();
        input();
    }

    cout << "Masukkan Password : ";
    getline(cin, users[indexData].password);
    while (!syaratpassword(users[indexData].password)){
        system("cls");
        cout << "Password harus memiliki 6 karakter dengan kombinasi\nhuruf kapital\nhuruf kecil\nangka." << endl;
        input();
    }
    users[indexData].saldo = 0;
}

void Regis(){
    system("cls");
    input();

    ifstream CheckUsers("Database/Data_User.csv");
    if (CheckUsers.is_open()) {
        string line;
        while (getline(CheckUsers, line)){
            stringstream ss(line);
            string username, password, role;
            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, role, ',');

            if (username == users[indexData].username) {
                cout << "Pengguna sudah ada" << endl;
                CheckUsers.close();
                getch();
                Menu();
                return;
            }
        }
        CheckUsers.close();
    }

    string opsi[2] = {"Admin", "Pelanggan"};
    string opsi_header = "Pilih Role";
    system("cls");
    int roleIndex = showmenu(2, opsi, opsi_header);
    if (roleIndex == 0){
        string passwordAdmin;
        cout << "Masukkan Password Admin : ";
        getline(cin, passwordAdmin);
        if (passwordAdmin != "admin")
        {
            cout << "Password Admin Salah" << endl;
            getch();
            Menu();
        }
        users[indexData].role = "Admin";
    }
    else{
        users[indexData].role = "Pelanggan";
    }

    ofstream database("Database/Data_User.csv", ios::app);

    database << users[indexData].username << "," << users[indexData].password << ","
             << users[indexData].role << "," << users[indexData].saldo << endl;
    database.close();
    system("cls");
    cout << "Registrasi Berhasil" << endl;
    indexData++;

    getch();
    Menu();
}

void Login(int &gagal){
    ReadFile();
    system("cls");
    string login_header = "LOGIN";
    string login[2] = {"Login", "Kembali"};
    int pilih = showmenu(2, login, login_header);
    if(pilih == 1){
        Menu();
    }

    cout << "LOGIN" << endl;
    cout << "Masukkan Username : ";
    getline(cin, inputUsername);
    cout << "Masukkan Password : ";
    getline(cin, inputPassword);
    if(inputUsername.empty() || inputPassword.empty()){
        cout << "Username atau Password tidak boleh kosong" << endl;
        gagal++;
        getch();
        Login(gagal);
    }

    for (int i = 0; i < indexData; i++){
        if (inputUsername == users[i].username && inputPassword == users[i].password){
            index_akun = i;
            system("cls");
            if (users[i].role == "Admin"){
                cout << "Selamat datang Admin " << users[i].username << endl;
                MenuAdmin();
            }
            else{
                cout << "HALOO " << users[i].username << endl;
                MenuPelanggan();
            }
            return;
        }
    }

    cout << "Username atau Password salah!" << endl;
    gagal++;
    if (gagal >=3){
        Menu();
    }
    getch();
    Login(gagal);
}

#endif
