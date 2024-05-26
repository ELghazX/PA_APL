#ifndef ADMIN_H
#define ADMIN_H

#include "akun.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <conio.h>
#include <sstream>
#include "UIUIX.h"

using namespace filesystem;
using namespace std;

int indexDataMenu = 0;
string inputID, inputNama, inputHarga, inputJenis;

struct daftar_menu{
    string id;
    string nama;
    string harga;
    string jenis;
};

daftar_menu menu[64];

struct Pesanan{
    string id_pesanan;
    string id_akun;
    string id_menu;
    string nama_menu;
    string harga;
    string tanggal;
    int dikonfirmasi;
};

Pesanan order[64];
int indexPesanan = 0;


void Menu();
void MenuAdmin();
void ReadFile();
void ReadMenu();
void TambahMenu();
void InsertionSortMenuID();
void InsertionSortMenuHarga();
void TampilkanMenu();
void UbahMenu();
void HapusMenu();
int InterpolationSearchMenu(const string &id);
void KonfirmasiPesanan();
void TampilkanPesanan();
void SavePesanan();
void SaveAkun();


void Read_Pesanan(){

    ifstream pesananFile("Database/Pesanan.csv");
    if(pesananFile.is_open()){
        string baris;
        getline(pesananFile, baris);
        indexPesanan = 0;
        while(getline(pesananFile, baris)){
            stringstream ss(baris);
            string field;
            int fieldIndex = 0;
            Pesanan pesanan;
            while(getline(ss, field, ',')){
                switch (fieldIndex){
                case 0:
                    pesanan.id_pesanan = field;
                    break;
                case 1:
                    pesanan.id_akun = field;
                    break;
                case 2:
                    pesanan.id_menu = field;
                    break;
                case 3:
                    pesanan.nama_menu = field;
                    break;
                case 4:
                    pesanan.harga = field;
                    break;
                case 5:
                    pesanan.tanggal = field;
                    break;
                case 6:
                    pesanan.dikonfirmasi = stoi(field);
                    break;
                }
                fieldIndex++;
            }
            order[indexPesanan++] = pesanan;
        }
    }
}


void ReadMenu(){
    ifstream daftarmenu("Database/Daftar_Menu.csv");
    if (daftarmenu.is_open()){
        string baris;
        getline(daftarmenu, baris);
        indexDataMenu = 0;
        while (getline(daftarmenu, baris)){
            stringstream ss(baris);
            string field;
            int fieldIndex = 0;
            daftar_menu menoe;
            while (getline(ss, field, ',')){
                switch (fieldIndex){
                case 0:
                    menoe.id = field;
                    break;
                case 1:
                    menoe.nama = field;
                    break;
                case 2:
                    menoe.harga = field;
                    break;
                case 3:
                    menoe.jenis = field;
                    break;
                }
                fieldIndex++;
            }
            menu[indexDataMenu++] = menoe;
        }
    }
    daftarmenu.close();
}
void SaveAkun(){
    ofstream akunFile("Database/Data_User.csv");
    akunFile << "Username,Password,Role,Saldo" << endl;
    for (int i = 0; i < indexData; i++){
        akunFile << users[i].username << "," << users[i].password << "," << users[i].role << "," << users[i].saldo << endl;
    }
    akunFile.close();
}

void SavePesanan(){
    ofstream pesanan("Database/Pesanan.csv");
    pesanan << "ID_Pesanan,ID_Akun,ID_Menu,Nama_Menu,Harga,Tanggal,Dikonfirmasi" << endl;
    for (int i = 0; i < indexPesanan; i++) {
        order[i].id_pesanan = to_string(i + 1);
        pesanan << order[i].id_pesanan << "," << order[i].id_akun << ","
            << order[i].id_menu << "," << order[i].nama_menu << ","
            << order[i].harga << "," << order[i].tanggal << ","
            << order[i].dikonfirmasi << endl;
    }
    pesanan.close();
}
void inputmenu(){
    string menu_header = "TAMBAH MENU";
    string menuopsi[2] = {"Tambah Menu", "Kembali"};
    int pilih = showmenu(2, menuopsi, menu_header);
    if(pilih == 1){
        MenuAdmin();
    }
    system("cls");
    cout <<"TAMBAH MENU"<<endl;
    cout << "Masukkan Nama Menu : ";
    getline(cin, menu[indexDataMenu].nama);
    trim(menu[indexDataMenu].nama);
    if (menu[indexDataMenu].nama.empty()){
        system("cls");
        cout << "Nama Menu tidak boleh kosong" << endl;
        inputmenu();
    }
    cout << "Masukkan Harga Menu : ";
    getline(cin, menu[indexDataMenu].harga);
    if (menu[indexDataMenu].harga.empty() || !isNumber(menu[indexDataMenu].harga)){
       
        cout << "Harga tidak valid" << endl;
        
        inputmenu();
    }
}

void TambahMenu(){
    system("cls");
    ReadMenu();
    inputmenu();

    string opsi[2] = {"makanan", "minuman"};
    string opsi_header = "Pilih Jenis Menu";
    int pilih = showmenu(2, opsi, opsi_header);
    menu[indexDataMenu].jenis = pilih == 0 ? "makanan" : "minuman";
    menu[indexDataMenu].id = to_string(indexDataMenu + 1);
    ofstream daftarmenu("Database/Daftar_Menu.csv", ios::app);
    daftarmenu << menu[indexDataMenu].id << "," << menu[indexDataMenu].nama << ","
               << menu[indexDataMenu].harga << "," << menu[indexDataMenu].jenis << endl;
    daftarmenu.close();
    system("cls");
    cout << "Data Berhasil Ditambahkan" << endl;
    indexDataMenu++;
    getch();
    MenuAdmin();
}

void TampilkanMenu(){
    ReadMenu();
    if(users[index_akun].role == "Admin"){
        InsertionSortMenuID();
    }else{
    InsertionSortMenuHarga();
    }
    system("cls");
    string opsi_header = "Daftar Menu:";
    string opsi[4] = {"Makanan", "Minuman", "Semua Menu", "Kembali"};
    int pilih = showmenu(4, opsi, opsi_header);
    system("cls");

    if (pilih == 0){
        cout << "Daftar makanan:" << endl;
        for (int i = 0; i < indexDataMenu; i++){
            if (menu[i].jenis == "makanan"){
                cout << "ID: " << menu[i].id << endl;
                cout << "Nama: " << menu[i].nama << endl;
                cout << "Harga: " << menu[i].harga << endl;
                cout << endl;
            }
        }
    }
    else if (pilih == 1){
        cout << "Daftar minuman:" << endl;
        for (int i = 0; i < indexDataMenu; i++){
            if (menu[i].jenis == "minuman") {
                cout << "ID: " << menu[i].id << endl;
                cout << "Nama: " << menu[i].nama << endl;
                cout << "Harga: " << menu[i].harga << endl;
                cout << endl;
            }
        }
    }
    else if (pilih == 2){
        cout << "Daftar Menu:" << endl;
        for (int i = 0; i < indexDataMenu; i++){
            cout << "ID: " << menu[i].id << endl;
            cout << "Nama: " << menu[i].nama << endl;
            cout << "Harga: " << menu[i].harga << endl;
            cout << "Jenis: " << menu[i].jenis << endl;
            cout << endl;
        }
    }else{
        if (users[index_akun].role == "Admin"){
            MenuAdmin();
        }else{
            MenuPelanggan();
        }
    }
    return;
}

void UbahMenu(){
    ReadMenu();
    system("cls");
    TampilkanMenu();
    string idbaru, namabaru, hargabaru, jenisbaru;
    cout << "\nMasukkan ID Menu yang ingin diubah: ";
    getline(cin, idbaru);
    if (idbaru.empty() || !isNumber(idbaru)){
        cout << "ID tidak valid" << endl;
        getch();
        UbahMenu();
    }
    int index = InterpolationSearchMenu(idbaru);

    if (index != -1){
        system("cls");
        cout << "Mengganti Menu: "<< menu[index].nama << endl<<endl;
        string ubahmenu_header = "UBAH MENU";
        string ubahmenu_opsi[2] = {"Konfirmasi", "Kembali"};
        int konfirm = showmenu(2, ubahmenu_opsi, ubahmenu_header);
        if (konfirm == 1){
            UbahMenu();
        }

        system("cls");
        cout << "Masukkan Nama Menu Baru: ";
        getline(cin, namabaru);
        trim(namabaru);
        if (namabaru.empty()){
            cout << "Nama Menu tidak boleh kosong" << endl;
            getch();
            UbahMenu();
        }
        cout << "Masukkan Harga Menu Baru: ";
        getline(cin, hargabaru);
        if (hargabaru.empty() || !isNumber(hargabaru)){
            cout << "Harga tidak valid" << endl;
            getch();
            UbahMenu();
        }
        string jenis_header = "Pilih Jenis Menu";
        string jenis_opsi[2] = {"makanan", "minuman"};
        int pilih = showmenu(2, jenis_opsi, jenis_header);
        jenisbaru = pilih == 0 ? "makanan" : "minuman";
        menu[index].nama = namabaru;
        menu[index].harga = hargabaru;
        menu[index].jenis = jenisbaru;
        
        ofstream database("Database/Daftar_Menu.csv");
        database << "ID_Menu,Nama_Menu,Harga_Menu,Jenis_Menu" << endl;
        for (int i = 0; i < indexDataMenu; i++){
            database << menu[i].id << "," << menu[i].nama << "," << menu[i].harga << "," << menu[i].jenis << endl;
        }
        database.close();
        cout << "Menu Berhasil Diubah" << endl;
        getch();
        MenuAdmin();
    }
    else{
        cout << "ID tidak ditemukan." << endl;
        getch();
        MenuAdmin();
    }

}

void HapusMenu(){
    ReadMenu();
    system("cls");
    TampilkanMenu();
    string idhapus;
    cout << "Masukkan ID Menu yang ingin dihapus: ";
    getline(cin, idhapus);
    system("cls");
    if (idhapus.empty() || !isNumber(idhapus)){
        cout << "ID tidak valid" << endl;
        getch();
        HapusMenu();
        return;
    }
    int index = InterpolationSearchMenu(idhapus);
    if (index != -1) {
        for (int j = index; j < indexDataMenu - 1; ++j){
            menu[j] = menu[j + 1];
        }
        indexDataMenu--;
    }
    else{
        cout << "ID tidak ditemukan" << endl;
        getch();
        MenuAdmin();
    }
    ofstream database("Database/Daftar_Menu.csv");
    database << "ID_Menu,Nama_Menu,Harga_Menu,Jenis_Menu" << endl; // write the header back
    for (int i = 0; i < indexDataMenu; i++) {
        database << menu[i].id << "," << menu[i].nama << "," << menu[i].harga << "," << menu[i].jenis << endl;
    }
    database.close();
    cout << "Data Berhasil Dihapus" << endl;
    getch();
    MenuAdmin();
}

void InsertionSortMenuHarga(){
    for (int i = 1; i < indexDataMenu; ++i){
        daftar_menu key = menu[i];
        int j = i - 1;
        while (j >= 0 && stoi(menu[j].harga) > stoi(key.harga)){
            menu[j + 1] = menu[j];
            j = j - 1;
        }
        menu[j + 1] = key;
    }
}
void InsertionSortMenuID(){
    for (int i = 1; i < indexDataMenu; ++i){
        daftar_menu key = menu[i];
        int j = i - 1;
        while (j >= 0 && stoi(menu[j].id) > stoi(key.id)){
            menu[j + 1] = menu[j];
            j = j - 1;
        }
        menu[j + 1] = key;
    }
}

void InsertionSortPesananID(){
    for (int i = 1; i < indexPesanan; ++i){
        Pesanan key = order[i];
        int j = i - 1;
        while (j >= 0 && stoi(order[j].id_pesanan) > stoi(key.id_pesanan)){
            order[j + 1] = order[j];
            j = j - 1;
        }
        order[j + 1] = key;
    }
}


int InterpolationSearchMenu(const string &id_menu) {
    int lo = 0, hi = indexDataMenu - 1;
    int target = stoi(id_menu);
    while (lo <= hi && target >= stoi(menu[lo].id) && target <= stoi(menu[hi].id)) {
        if (lo == hi) {
            if (stoi(menu[lo].id) == target)
                return lo;
            return -1;
        }
        int pos = lo + ((double)(hi - lo) / (stoi(menu[hi].id) - stoi(menu[lo].id)) * (target - stoi(menu[lo].id)));
        if (pos < lo || pos > hi)
            return -1;

        if (stoi(menu[pos].id) == target)
            return pos;

        if (stoi(menu[pos].id) < target)
            lo = pos + 1;
        else
            hi = pos - 1;
    }
    return -1;
}

int InterpolationSearchPesanan(const string &id) {
    int lo = 0, hi = indexPesanan - 1;
    int target = stoi(id);
    while (lo <= hi && target >= stoi(order[lo].id_pesanan) && target <= stoi(order[hi].id_pesanan)) {
        if (lo == hi) {
            if (stoi(order[lo].id_pesanan) == target)
                return lo;
            return -1;
        }

        int pos = lo + ((double)(hi - lo) / (stoi(order[hi].id_pesanan) - stoi(order[lo].id_pesanan)) * (target - stoi(order[lo].id_pesanan)));

        if (pos < lo || pos > hi)
            return -1;

        if (stoi(order[pos].id_pesanan) == target)
            return pos;

        if (stoi(order[pos].id_pesanan) < target)
            lo = pos + 1;
        else
            hi = pos - 1;
    }
    return -1;
}


void KonfirmasiPesanan(){
    Read_Pesanan();

    system("cls");
    string opsi[2] = {"Konfirmasi", "Kembali"};
    string opsi_header = "Konfirmasi Pesanan";
    int pilih = showmenu(2, opsi, opsi_header);
    if (pilih == 1){
        MenuAdmin();
    }
    if(indexPesanan == 0){
        cout << "Tidak ada pesanan yang perlu dikonfirmasi" << endl;
        getch();
        KonfirmasiPesanan();
    }
    bool mengkonfirm = false;

    for (int i = 0; i < indexPesanan; i++){
        if (order[i].dikonfirmasi == 0){
            mengkonfirm = true;
            cout << "ID Pesanan: " << order[i].id_pesanan << endl;
            cout << "ID Akun: " << order[i].id_akun << endl;
            cout << "ID Menu: " << order[i].id_menu << endl;
            cout << "Nama Menu: " << order[i].nama_menu << endl;
            cout << "Harga: " << order[i].harga << endl;
            cout << "Tanggal: " << order[i].tanggal << endl;
            cout << "Status: Belum dikonfirmasi" << endl;
            cout << endl;
        }
    }
    if (!mengkonfirm){
        cout << "Tidak ada pesanan yang perlu dikonfirmasi" << endl;
        getch();
        KonfirmasiPesanan();
    }
    string IDpesanan;
    cout << "Masukkan ID Pesanan yang ingin dikonfirmasi: ";
    getline(cin, IDpesanan);
    if (IDpesanan.empty() || !isNumber(IDpesanan) ){
        cout << "ID Pesanan tidak valid" << endl;
        getch();
        KonfirmasiPesanan();
    }
    int index = InterpolationSearchPesanan(IDpesanan);
    if (index != -1 && order[index].dikonfirmasi == 0){
        order[index].dikonfirmasi = 1;
        cout << "Pesanan berhasil dikonfirmasi" << endl;
        
        SavePesanan();
    }
    else{
        cout << "ID Pesanan tidak ditemukan" << endl;
        getch();
        KonfirmasiPesanan();
    } 

    mengkonfirm = false;
    for (int i; i < indexPesanan; i++){
        if (order[i].dikonfirmasi == 0){
            mengkonfirm = true;
        }
    }
}

void TampilkanPesanan(){
    Read_Pesanan();
    system("cls");
    bool adaPesananTerkonfirmasi = false;

    cout << "Pesanan Terkonfirmasi" << endl
         << endl;

    for (int i = 0; i < indexPesanan; i++){
        if (order[i].dikonfirmasi == 1){
            adaPesananTerkonfirmasi = true;
            cout << "ID Pesanan: " << order[i].id_pesanan << endl;
            cout << "ID Menu: " << order[i].id_menu << endl;
            cout << "Nama Menu: " << order[i].nama_menu << endl;
            cout << "Harga: " << order[i].harga << endl;
            cout << "Tanggal: " << order[i].tanggal << endl;
            cout << endl;
        }
    }

    if (!adaPesananTerkonfirmasi){
        cout << "Tidak ada pesanan yang terkonfirmasi" << endl;
    }

    cout << "Tekan tombol apapun untuk lanjut" << endl;
    getch();
    MenuAdmin();
}

void MenuAdmin(){
    system("cls");
    string opsi[7] = {"Tambah Menu", "Tampilkan Daftar Menu", "Ubah Daftar Menu", "Hapus Daftar Menu", "Konfirmasi Pesanan", "Tampilkan Pesanan", "Logout"};
    string opsi_header = "MENU ADMIN";

    int pilih = showmenu(7, opsi, opsi_header);
    
    if (pilih == 0){
        TambahMenu();
    }
    else if (pilih == 1){
        InsertionSortMenuID();
        TampilkanMenu();
        getch();
        MenuAdmin();
    }
    else if (pilih == 2) {
        InsertionSortMenuID();
        UbahMenu();
    }
    else if (pilih == 3){
        InsertionSortMenuID();
        HapusMenu();
    }
    else if (pilih == 4) {
        InsertionSortMenuID();
        KonfirmasiPesanan();
        MenuAdmin();
    }
    else if (pilih == 5){
        InsertionSortMenuID();
        TampilkanPesanan();
    }
    else {
        Menu();
    }
}

#endif
