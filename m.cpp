
#include <iostream>
#include <string>

using namespace std;

struct Dompet
{
    double saldo;
};

struct HistoryTopup
{
    string namaPengguna;
    double jumlahTopup;
};

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

void tambahHistoryTopup(HistoryTopup historyTopup[], int &topupIndex, const string &namaPengguna, double jumlahTopup)
{
    historyTopup[topupIndex].namaPengguna = namaPengguna;
    historyTopup[topupIndex].jumlahTopup = jumlahTopup;
    ++topupIndex;
}

void cetakHistoryTopup(const HistoryTopup historyTopup[], int topupIndex, const string &namaPengguna)
{
    bool found = false;
    cout << "Nama Pengguna\tJumlah Topap\n";
    cout << "-----------------------------\n";

    for (int i = 0; i < topupIndex; ++i)
    {
        if (historyTopup[i].namaPengguna == namaPengguna)
        {
            cout << historyTopup[i].namaPengguna << "\t" << historyTopup[i].jumlahTopup << endl;
            found = true;
        }
    }
    if (!found)
    {
        cout << "Tidak ada riwayat top-up untuk " << namaPengguna << endl;
    }
}

void prosesTopup(Dompet &dompet, HistoryTopup historyTopup[], int &topupIndex, const string &namaPengguna)
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
        tambahHistoryTopup(historyTopup, topupIndex, namaPengguna, jumlah);
    }
    else
    {
        cout << "Topap dibatalkan.\n";
    }
}

void tampilkanMenu(Dompet &dompet, HistoryTopup historyTopup[], int &topupIndex, const string &namaPengguna)
{
    string input;
    while (true)
    {
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
            prosesTopup(dompet, historyTopup, topupIndex, namaPengguna);
        }
        else if (input == "2")
        {
            cout << "Saldo saat ini: " << dompet.saldo << endl;
        }
        else if (input == "3")
        {
            cetakHistoryTopup(historyTopup, topupIndex, namaPengguna);
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
    string namaPengguna;
    const int maxHistory = 100;
    HistoryTopup historyTopup[maxHistory];

    int topupIndex = 0;

    cout << "Masukkan nama pengguna: ";
    cin >> namaPengguna;
    tampilkanMenu(dompetSaya, historyTopup, topupIndex, namaPengguna);

    return 0;
}
