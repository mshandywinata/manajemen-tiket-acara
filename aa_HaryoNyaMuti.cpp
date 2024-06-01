#include <iostream>
#include <string>
#include <conio.h>

using namespace std;

const int MAKS_PENGGUNA = 100;
const int MAX_HISTORY = 100;

struct DataPengguna
{
    string namaPengguna, kataSandi, posisi;
};

struct Dompet
{
    double saldo;
};

struct HistoryTopup
{
    string namaPengguna;
    double jumlahTopup;
};

DataPengguna penggunaAktif[MAKS_PENGGUNA];
HistoryTopup historyTopup[MAX_HISTORY];
int jumlahPengguna = 0;
int topupIndex = 0;

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

    // cek nama apakah ada
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

void isiSaldo(Dompet &dompet, double jumlah)
{
    if (jumlah > 0)
    {
        dompet.saldo += jumlah;
        cout << "Top-up berhasil! Saldo baru: " << dompet.saldo << endl;
    }
    else
    {
        cout << "Jumlah harus positif." << endl;
    }
}

void tambahHistoryTopup(const string &namaPengguna, double jumlahTopup)
{
    if (topupIndex < MAX_HISTORY)
    {
        historyTopup[topupIndex].namaPengguna = namaPengguna;
        historyTopup[topupIndex].jumlahTopup = jumlahTopup;
        ++topupIndex;
    }
    else
    {
        cout << "Riwayat top-up penuh!" << endl;
    }
}

void cetakHistoryTopup(const string &namaPengguna)
{
    bool ditemukan = false;
    cout << "Nama Pengguna\tJumlah Topap\n";
    cout << "-----------------------------\n";

    for (int i = 0; i < topupIndex; ++i)
    {
        if (historyTopup[i].namaPengguna == namaPengguna)
        {
            cout << historyTopup[i].namaPengguna << "\t" << historyTopup[i].jumlahTopup << endl;
            ditemukan = true;
        }
    }
    if (!ditemukan)
    {
        cout << "Tidak ada riwayat top-up untuk " << namaPengguna << endl;
    }
}

void prosesTopup(Dompet &dompet, const string &namaPengguna)
{
    double jumlah;
    string input;
    cout << "Masukkan jumlah untuk top-up: ";
    cin >> jumlah;
    cout << "Anda akan menambah saldo sebesar: " << jumlah << ". Konfirmasi (y/n): ";
    cin >> input;
    if (input == "y" || input == "Y")
    {
        isiSaldo(dompet, jumlah);
        tambahHistoryTopup(namaPengguna, jumlah);
    }
    else
    {
        cout << "Topap dibatalkan.\n";
    }
}

void tampilkanMenu(Dompet &dompet, const string &namaPengguna)
{
    string input;
    while (true)

    {
        cout << "=============================";
        cout << "\n";
        cout << "Pilih opsi buat berak:\n";
        cout << "1. Topap saldo\n";
        cout << "2. Periksa saldo\n";
        cout << "3. Lihat riwayat \n";
        cout << "Ketik 'keluar' untuk mengakhiri\n";
        cout << "Masukkan pilihan Anda: ";
        cin >> input;

        if (input == "keluar")
        {
            break;
        }
        else if (input == "1")
        {
            prosesTopup(dompet, namaPengguna);
        }
        else if (input == "2")
        {
            cout << "Saldo saat ini: " << dompet.saldo << endl;
        }
        else if (input == "3")
        {
            cetakHistoryTopup(namaPengguna);
        }
        else
        {
            cout << "Pilihan tidak valid." << endl;
        }
    }
}

int main()
{
    Dompet dompetSaya = {0.0};
    DataPengguna penggunaSaatIni;
    int pilihan;

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
                cout << "Selamat datang, " << penggunaSaatIni.namaPengguna << " (" << penggunaSaatIni.posisi << ")" << endl;
                tampilkanMenu(dompetSaya, penggunaSaatIni.namaPengguna);
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
