#ifndef PELANGGAN_H
#define PELANGGAN_H

#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include "UIUIX.h"
#include "akun.h"
#include "admin.h"
string GetTanggal();
using namespace std;

void ReadMenu();
void CekSaldo();
void PesanMenu();
void BatalkanPesanan();
void TampilkanRiwayat();

void CekSaldo(){
    ReadFile();
    system("cls");

    cout << "Saldo Anda: " << users[index_akun].saldo << endl;
    string opsi[2] = {"Top Up Saldo", "Kembali"};
    string opsi_header = "MENU SALDO";
    int pilihan = showmenu(2, opsi, opsi_header);
    if (pilihan == 0){
        int saldo;
        cout << "Saldo Anda: " << users[index_akun].saldo << endl;
        string saldo_header = "Masukkan jumlah saldo yang ingin diisi: ";
        string saldo_opsi[] = {"20000", "50000", "200000", "500000"};
        int pilihan = showmenu(4, saldo_opsi, saldo_header);
        if (pilihan == 0)  {
            saldo = 20000;
        }
        else if (pilihan == 1) {
            saldo = 50000;
        }
        else if (pilihan == 2) {
            saldo = 200000;
        }
        else{
            saldo = 500000;
        }
        users[index_akun].saldo += saldo;

        ofstream database("Database/Data_User.csv");
        database << "Username,Password,Role,Saldo" << endl;
        for (int i = 0; i < indexData; i++){
            database << users[i].username << "," << users[i].password << "," << users[i].role << "," << users[i].saldo << endl;
        }
        cout << "Saldo berhasil diisi" << endl;
    }
    else{
        MenuPelanggan();
    }
    cout << "Saldo Anda: " << users[index_akun].saldo << endl;
    cout << "Tekan enter untuk kembali ke menu pelanggan" << endl;
    getch();
    MenuPelanggan();
}

void PesanMenu() {
    ReadMenu();
    ReadFile();
    Read_Pesanan();
    system("cls");
    TampilkanMenu();

    string idPesan;
    cout << "Masukkan ID Menu yang ingin dipesan: ";
    getline(cin, idPesan);
    if (idPesan.empty() || !isNumber(idPesan)) {
        cout << "ID Menu tidak valid" << endl;
        getch();
        PesanMenu();
    }

    InsertionSortMenuID();
    int index = InterpolationSearchMenu(idPesan);
    if (index != -1) {
        int harga = stoi(menu[index].harga);

        if (users[index_akun].saldo >= harga) {
            system("cls");
            cout << "Memesan" << endl;
            cout << "Nama Menu: " << menu[index].nama << endl;
            cout << "Harga: " << menu[index].harga << endl;
            cout << "Jenis: " << menu[index].jenis << endl;

            users[index_akun].saldo -= harga;

            order[indexPesanan].id_akun = to_string(index_akun);
            order[indexPesanan].id_menu = menu[index].id;
            order[indexPesanan].id_pesanan = to_string(indexPesanan + 1);
            order[indexPesanan].nama_menu = menu[index].nama;
            order[indexPesanan].harga = menu[index].harga;
            order[indexPesanan].tanggal = GetTanggal();
            order[indexPesanan].dikonfirmasi = 0;

            InsertionSortPesananID();
            ofstream pesanan("Database/Pesanan.csv", ios::app);
            pesanan << order[indexPesanan].id_pesanan << "," << order[indexPesanan].id_akun << ","
                << order[indexPesanan].id_menu << "," << order[indexPesanan].nama_menu << ","
                << order[indexPesanan].harga << "," << order[indexPesanan].tanggal << ","
                << order[indexPesanan].dikonfirmasi << endl;
            pesanan.close();

            ofstream data("Database/Data_User.csv");
            data << "Username,Password,Role,Saldo" << endl;
            for (int i = 0; i < indexData; i++) {
                data << users[i].username << "," << users[i].password << "," << users[i].role << "," << users[i].saldo << endl;
            }
            data.close();
            indexPesanan++;
        }
        else {
            system("cls");
            cout << "Saldo tidak cukup untuk melakukan pemesanan." << endl;
        }
    }
    else {
        cout << "ID Menu tidak ditemukan." << endl;
    }
    getch();
    system("cls");
    string opsi[2] = { "Pesan lagi", "Kembali" };
    string opsi_header = "MENU PESAN";
    int pilihan = showmenu(2, opsi, opsi_header);
    if (pilihan == 0) {
        PesanMenu();
    }
    else {
        MenuPelanggan();
    }
}

void BatalkanPesanan() {
    Read_Pesanan();
    ReadMenu();
    ReadFile();

    system("cls");
    string batal_header = "Batalkan Pesanan";
    string batal_opsi[2] = { "Batalkan Pesanan", "Kembali" };
    int pilih = showmenu(2, batal_opsi, batal_header);
    if (pilih == 1) {
        MenuPelanggan();
    }
    if (indexPesanan == 0) {
        cout << "Tidak ada pesanan yang dapat dibatalkan" << endl;
        getch();
        PesanMenu();
    }

    bool bisabatal = false;
    for (int i = 0; i < indexPesanan; i++) {
        if (order[i].dikonfirmasi == 0 && stoi(order[i].id_akun) == index_akun) {
            bisabatal = true;
            cout << "ID Pesanan: " << order[i].id_pesanan << endl;
            cout << "Nama Menu: " << order[i].nama_menu << endl;
            cout << "Harga: " << order[i].harga << endl;
            cout << "Tanggal: " << order[i].tanggal << endl;
            cout << "Status: Belum dikonfirmasi" << endl;
            cout << endl;
        }
    }
    if (!bisabatal) {
        cout << "Tidak ada pesanan yang dapat dibatalkan" << endl;
        getch();
        MenuPelanggan();
    }

    string cariidpesanan;
    cout << "Masukkan ID Pesanan yang ingin dibatalkan: ";
    getline(cin, cariidpesanan);
    if (cariidpesanan.empty() || !isNumber(cariidpesanan)) {
        cout << "ID Pesanan tidak valid" << endl;
        getch();
        BatalkanPesanan();
    }
    else {

        InsertionSortPesananID();
        int index = InterpolationSearchPesanan(cariidpesanan);
        system("cls");
        if (index != -1 && order[index].dikonfirmasi == 0) {
            users[index_akun].saldo += stoi(order[index].harga);
            for (int j = index; j < indexPesanan - 1; ++j) {
                order[j] = order[j + 1];
            }

            cout << "Pesanan " << order[index].nama_menu << " berhasil dibatalkan" << endl;
            cout << "Saldo " << order[index].harga << " berhasil dikembalikan" << endl;
            indexPesanan--;
        }
        else {
            cout << "ID Pesanan tidak ditemukan" << endl;
            getch();
            BatalkanPesanan();
        }
        bisabatal = false;
        for (int i = 0; i < indexPesanan; i++) {
            if (order[i].dikonfirmasi == 0) {
                bisabatal = true;
            }
        }
        getch();
        SaveAkun();
        InsertionSortPesananID();
        SavePesanan();
        MenuPelanggan();
    }
}


void TampilkanRiwayat(){
    system("cls");
    Read_Pesanan();
    ReadMenu();
    cout << "Riwayat Pemesanan" << endl<<endl;
    if(indexPesanan == 0){
        cout << "Tidak ada riwayat pemesanan" << endl<<endl;
        cout << "Tekan enter untuk kembali ke menu pelanggan" << endl;
        getch();
        MenuPelanggan();
    }
    for (int i = 0; i < indexPesanan; i++){
        if (stoi(order[i].id_akun) == index_akun){
            cout << "ID Pesanan: " << order[i].id_pesanan << endl;
            cout << "Nama Menu: " << order[i].nama_menu << endl;
            cout << "Harga: " << order[i].harga << endl;
            cout << "Tanggal: " << order[i].tanggal << endl;
            if (order[i].dikonfirmasi == 0){
                cout << "Status: Belum dikonfirmasi" << endl;
            }
            else{
                cout << "Status: Sudah dikonfirmasi" << endl;
            }   
            cout << endl;
        }
    }

    cout << "Tekan enter untuk kembali ke menu pelanggan" << endl;
    
    getch();
    MenuPelanggan();
}

string GetTanggal(){
    time_t now = time(0);
    tm *ltm = localtime(&now);
    stringstream ss;
    ss << 1900 + ltm->tm_year << "-" << 1 + ltm->tm_mon
    << "-" << ltm->tm_mday;
    return ss.str();
}

void MenuPelanggan(){
    system("cls");
    string opsi[5] = {"ISI/CHECK SALDO", "PESAN MENU", "BATALKAN PESANAN", "LIHAT RIWAYAT PEMESANAN", "LOGOUT"};
    string opsi_header = "MENU PELANGGAN";

    int pilih = showmenu(5, opsi, opsi_header);

    if (pilih == 0){
        CekSaldo();
    }
    else if (pilih == 1){
        
        PesanMenu();
    }
    else if (pilih == 2){
        InsertionSortMenuID();
        BatalkanPesanan();
    }
    else if (pilih == 3){
        InsertionSortMenuID();
        TampilkanRiwayat();
    }
    else{
        Menu();
    }

    getch();
    MenuPelanggan();
}
#endif
