



#include <iostream>
#include <string>
#include <ctime>
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

// Struktur Kursi untuk menyimpan kursi
struct Kursi
{
    int slotKursi[100];
    int top;
};

// Struktur Event
struct Event
{
    string nama;
    string tanggal;
    string kelasTiket[3];
    int harga[3];
    Kursi kursi[3];
    Event *kiri;
    Event *kanan;
    time_t waktu;
};

// Fungsi untuk membuat Event baru
Event *buatEvent(const string &nama, const string &tanggal, int slotEkonomi, int slotReguler, int slotVIP, int hargaEkonomi, int hargaReguler, int hargaVIP)
{
    Event *eventBaru = new Event;
    eventBaru->nama = nama;
    eventBaru->tanggal = tanggal;
    eventBaru->kelasTiket[0] = "ekonomi";
    eventBaru->kelasTiket[1] = "reguler";
    eventBaru->kelasTiket[2] = "VIP";
    eventBaru->harga[0] = hargaEkonomi;
    eventBaru->harga[1] = hargaReguler;
    eventBaru->harga[2] = hargaVIP;
    eventBaru->kursi[0] = {{}, -1};
    eventBaru->kursi[1] = {{}, -1};
    eventBaru->kursi[2] = {{}, -1};
    eventBaru->waktu = time(nullptr);

    for (int i = 0; i < slotEkonomi; ++i)
    {
        eventBaru->kursi[0].slotKursi[++eventBaru->kursi[0].top] = i + 1;
    }
    for (int i = 0; i < slotReguler; ++i)
    {
        eventBaru->kursi[1].slotKursi[++eventBaru->kursi[1].top] = i + 1;
    }
    for (int i = 0; i < slotVIP; ++i)
    {
        eventBaru->kursi[2].slotKursi[++eventBaru->kursi[2].top] = i + 1;
    }

    eventBaru->kiri = nullptr;
    eventBaru->kanan = nullptr;
    return eventBaru;
}

// Fungsi untuk menambah Event ke dalam pohon biner
Event *tambahEvent(Event *root, const string &nama, const string &tanggal, int slotEkonomi, int slotReguler, int slotVIP, int hargaEkonomi, int hargaReguler, int hargaVIP)
{
    if (root == nullptr)
    {
        return buatEvent(nama, tanggal, slotEkonomi, slotReguler, slotVIP, hargaEkonomi, hargaReguler, hargaVIP);
    }
    if (tanggal < root->tanggal)
    {
        root->kiri = tambahEvent(root->kiri, nama, tanggal, slotEkonomi, slotReguler, slotVIP, hargaEkonomi, hargaReguler, hargaVIP);
    }
    else
    {
        root->kanan = tambahEvent(root->kanan, nama, tanggal, slotEkonomi, slotReguler, slotVIP, hargaEkonomi, hargaReguler, hargaVIP);
    }
    return root;
}

// Fungsi untuk menampilkan event dengan nomor dan tampilan seperti tabel
void tampilkanEventTable(Event *root, int &nomor)
{
    if (root != nullptr)
    {
        tampilkanEventTable(root->kiri, nomor);
        cout << "| " << nomor++ << " | "
             << root->nama << " | "
             << root->tanggal << " | "
             << root->kelasTiket[0] << ": " << root->harga[0] << " | "
             << root->kelasTiket[1] << ": " << root->harga[1] << " | "
             << root->kelasTiket[2] << ": " << root->harga[2] << " |\n";
        tampilkanEventTable(root->kanan, nomor);
    }
}

// TAMBAH EVENT
void inputEvent(Event *&root)
{
    string nama;
    string tanggal;
    int slotEkonomi, slotReguler, slotVIP;
    int hargaEkonomi, hargaReguler, hargaVIP;

    cout << "=========================================\n";
    cout << "=           Tambah Event Baru           =\n";
    cout << "=========================================\n";
    cout << "\n";

    cout << "- Nama Event\t: ";
    getline(cin, nama);
    cout << "- Tanggal Event (yyyy-mm-dd)\t: ";
    getline(cin, tanggal);

    cout << "- Jumlah Slot Kursi Ekonomi \t: ";
    cin >> slotEkonomi;
    cout << "  Harga\t: ";
    cin >> hargaEkonomi;
    cout << "- Jumlah Slot Kursi Reguler \t: ";
    cin >> slotReguler;
    cout << "  Harga\t: ";
    cin >> hargaReguler;
    cout << "- Jumlah Slot Kursi VIP     \t: ";
    cin >> slotVIP;
    cout << "  Harga\t: ";
    cin >> hargaVIP;
    cin.ignore();

    root = tambahEvent(root, nama, tanggal, slotEkonomi, slotReguler, slotVIP, hargaEkonomi, hargaReguler, hargaVIP);

    cout << "Berhasil menambahkan event baru dengan " << slotEkonomi << " kursi ekonomi, " << slotReguler << " kursi reguler, dan " << slotVIP << " kursi VIP\n\n";
}

// Fungsi untuk menampilkan header tabel
void tampilkanHeaderTabel()
{
    cout << "+-----+------------------------------+--------------+------------------------+------------------------+------------------------+\n";
    cout << "| No  | Nama Event                   | Tanggal      | Ekonomi: Harga         | Reguler: Harga         | VIP: Harga             |\n";
    cout << "+-----+------------------------------+--------------+------------------------+------------------------+------------------------+\n";
}

// Fungsi untuk menampilkan footer tabel
void tampilkanFooterTabel()
{
    cout << "+-----+------------------------------+--------------+------------------------+------------------------+------------------------+\n";
}

int main()
{
    Event *root = nullptr;
    DataPengguna penggunaSaatIni;
    int pilihan;

    do
    {
        cout << "\n=== MENU NGAJAK RIBUT 2.0 ===\n\n";
        cout << "1. Daftar sebagai Pembeli\n";
        cout << "2. Daftar sebagai Penjual\n";
        cout << "3. Login\n";
        cout << "4. Tambah Event Baru\n";
        cout << "5. Tampilkan Event\n";
        cout << "6. Keluar\n";
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
            }
        }
        else if (pilihan == 4)
        {
            if (penggunaSaatIni.posisi == "Penjual")
            {
                inputEvent(root);
            }
            else
            {
                cout << "Hanya penjual yang dapat menambah event." << endl;
            }
        }
        else if (pilihan == 5)
        {
            int nomor = 1;
            tampilkanHeaderTabel();
            tampilkanEventTable(root, nomor);
            tampilkanFooterTabel();
        }
        else if (pilihan == 6)
        {
            cout << "Terima kasih telah menggunakan aplikasi kami!" << endl;
        }
        else
        {
            cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 6);

    return 0;
}
