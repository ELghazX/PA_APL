#include "Headers/menu_main.h"
#include "Headers/pelanggan.h"
#include "Headers/admin.h"
#include "Headers/akun.h"

void CekFile(){
    path p = current_path();
    if (!exists(p / "Database")){
        create_directory(p / "Database");
    }

    if (!exists(p / "Database/Data_User.csv")) {
        ofstream database;
        database.open("Database/Data_User.csv");
        database << "Username,Password,Role,Saldo" << endl;
        database.close();
    }
    if (!exists(p / "Database/Daftar_Menu.csv")){
    ofstream database;
    database.open("Database/Daftar_Menu.csv");
    database << "ID_Menu,Nama_Menu,Harga_Menu,Jenis_Menu" << endl;
    database.close();
    }
    if (!exists(p / "Database/Pesanan.csv")){
        ofstream database;
        database.open("Database/Pesanan.csv");
        database << "ID_Pesanan,ID_Akun,ID_Menu,Nama_Menu,Harga,Tanggal,Dikonfirmasi" << endl;
        database.close();
    }
}

int main(){
    CekFile();
    Menu();
    return 0;
}
