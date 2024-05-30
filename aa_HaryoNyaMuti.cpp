#include <iostream>
#include <string>
#include <conio.h>
using namespace std;

const int MAKS_PENGGUNA = 100;

struct DataPengguna
{
    string namaPengguna, kataSandi, posisi;
};
DataPengguna penggunaAktif[MAKS_PENGGUNA];
int jumlahPengguna = 0;

// FITUR APLIKASI

// FITUR BUAT REGIS GES
void daftarPengguna(const string &posisi)
{
    string namaPengguna, kataSandi;

    if (jumlahPengguna >= MAKS_PENGGUNA)
    {
        cout << "Kapasitas pengguna penuh, Tidak bisa menambah pengguna baru." << endl;
        return;
    }
    cout << "Masukkan nama pengguna: ";
    cin >> namaPengguna;
    cout << "Masukkan kata sandi: ";
    char karakterRahasia;
    kataSandi.clear();
    while ((karakterRahasia = _getch()) != '\r')
    {
        if (karakterRahasia == '\b')
        {
            if (!kataSandi.empty())
            {
                cout << "\b \b";
                kataSandi.pop_back();
            }
        }
        else
        {
            cout << '*';
            kataSandi += karakterRahasia;
        }
    }

    // simpen pengguna
    penggunaAktif[jumlahPengguna] = {namaPengguna, kataSandi, posisi};
    jumlahPengguna++;
    cout << endl
         << "Registrasi sebagai " << posisi << " berhasil" << endl;
}

// FUNGSI BUAT LOGIN GES
bool loginPengguna(DataPengguna &penggunaSaatIni)
{
    string namaPengguna, kataSandi;

    cout << "Masukkan nama pengguna: ";
    cin >> namaPengguna;
    cout << "Masukkan kata sandi: ";
    char karakterRahasia;
    kataSandi.clear();
    while ((karakterRahasia = _getch()) != '\r')
    {
        if (karakterRahasia == '\b')
        {
            if (!kataSandi.empty())
            {
                cout << "\b \b";
                kataSandi.pop_back();
            }
        }
        else
        {
            cout << '*';
            kataSandi += karakterRahasia;
        }
    }
    //    cek nama apakah ada
    for (int i = 0; i < jumlahPengguna; ++i)
    {
        if (penggunaAktif[i].namaPengguna == namaPengguna && penggunaAktif[i].kataSandi == kataSandi)
        {
            cout << endl
                 << "Login berhasil sebagai " << penggunaAktif[i].posisi << endl;
            penggunaSaatIni = penggunaAktif[i];
            return true;
        }
    }
    cout << endl
         << "Nama pengguna atau kata sandi salah" << endl;
    return false;
}

// MAIN
int main()
{
    int pilihan;
    DataPengguna penggunaSaatIni;

    do
    {

        cout << "\n=== MENU NGAJAK RIBUT 2.0 ===\n\n";
        cout << "1. Daftar sebagai Pembeli\n";
        cout << "2. Daftar sebagai Penjual\n";
        cout << "3. Login\n";
        cout << "4. Keluar\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        if (pilihan == 1)
        {
            daftarPengguna("Pembeli");
        }
        else if (pilihan == 2)
        {
            daftarPengguna("Penjual");
        }
        else if (pilihan == 3)
        {
            if (loginPengguna(penggunaSaatIni))
            {
                cout << "Selamat datang, " << penggunaSaatIni.namaPengguna << penggunaSaatIni.posisi << endl;
            }
        }
        else if (pilihan == 4)
        {
            cout << "Terima kasih telah menggunakan aplikasi kami!" << endl;
        }
        else
        {
            cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 4);

    return 0;
}
