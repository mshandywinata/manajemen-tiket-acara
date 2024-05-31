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

struct Stack
{
    int elements[100];
    int topIndex;

    Stack() : topIndex(-1) {}

    void push(int item)
    {
        if (topIndex < 99)
        {
            elements[++topIndex] = item;
        }
        else
        {
            cout << "Kursi sudah penuh" << endl;
        }
    }

    void pop()
    {
        if (topIndex >= 0)
        {
            --topIndex;
        }
        else
        {
            cout << "kursi masih tersedia " << endl;
        }
    }

    int &top()
    {
        if (topIndex >= 0)
        {
            return elements[topIndex];
        }
        else
        {
            throw out_of_range("Stack::top(): empty stack");
        }
    }

    bool empty() const
    {
        return topIndex == -1;
    }

    int size() const
    {
        return topIndex + 1;
    }
};

struct Kursi
{
    Stack slotKursi;
    int harga;
};

struct Event
{
    string namaEvent;
    string tanggalEvent;
    Kursi kelasEvent[3];
    string namaKelas[3] = {"ekonomi", "reguler", "vip"};
};

struct Graph
{
    Event listEvent[10];
    int eventCount = 0;

    void tambahEvent(const Event &event)
    {
        if (eventCount < 10)
        {
            listEvent[eventCount++] = event;
            cout << "==> Berhasil menambahkan event " << event.namaEvent << endl;
        }
        else
        {
            cout << "Maksimal event tercapai" << endl;
        }
    }

    void tampilkanEvents()
    {
        cout << ">>> List event yang ada: " << endl;
        for (int i = 0; i < eventCount; ++i)
        {
            cout << i + 1 << ". " << listEvent[i].namaEvent << " - " << listEvent[i].tanggalEvent << endl;
        }
    }

    void tampilkanDetailEvent(int index)
    {
        if (index < 0 || index >= eventCount)
        {
            cout << "Event tidak ditemukan!" << endl;
            return;
        }
        Event &event = listEvent[index];
        cout << "Nama Event   : " << event.namaEvent << endl;
        cout << "Tanggal Event: " << event.tanggalEvent << endl;
        for (int i = 0; i < 3; ++i)
        {
            cout << "Kelas " << event.namaKelas[i] << " - ";
            cout << "Harga: " << event.kelasEvent[i].harga << ", ";
            cout << "Jumlah Kursi: " << event.kelasEvent[i].slotKursi.size() << endl;
        }
    }
};

Event buatEventBaru()
{
    Event event;
    cout << "=======================================" << endl;
    cout << "=          Tambah Event Baru          =" << endl;
    cout << "=======================================" << endl;
    cout << "- Nama event         : ";
    getline(cin >> ws, event.namaEvent);
    cout << "- Tanggal event      : ";
    getline(cin, event.tanggalEvent);

    for (int i = 0; i < 3; ++i)
    {
        int slot;
        cout << "- Jumlah slot kursi " << event.namaKelas[i] << " : ";
        cin >> slot;
        cin.ignore();
        for (int j = 0; j < slot; ++j)
        {
            event.kelasEvent[i].slotKursi.push(1);
        }
        cout << "  Harga: ";
        cin >> event.kelasEvent[i].harga;
        cin.ignore();
    }

    return event;
}

// MAIN
int main()
{
    int pilihan;
    DataPengguna penggunaSaatIni;
    Graph graph;

    do
    {
        cout << "\n=== MENU NGAJAK RIBUT 2.0 ===\n\n";
        cout << "1. Daftar sebagai Pembeli\n";
        cout << "2. Daftar sebagai Penjual\n";
        cout << "3. Login\n";
        cout << "4. Tambah Event Baru\n";
        cout << "5. Tampilkan Event\n";
        cout << "6. Tampilkan Detail Event\n";
        cout << "7. Keluar\n";
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
                Event event = buatEventBaru();
                graph.tambahEvent(event);
            }
            else
            {
                cout << "Hanya penjual yang dapat menambah event." << endl;
            }
        }
        else if (pilihan == 5)
        {
            graph.tampilkanEvents();
        }
        else if (pilihan == 6)
        {
            if (penggunaSaatIni.posisi == "Penjual")
            {
                int index;
                graph.tampilkanEvents();
                cout << "Pilih nomor event untuk melihat detail: ";
                cin >> index;
                graph.tampilkanDetailEvent(index - 1);
            }
            else
            {
                cout << "Hanya penjual yang dapat melihat detail event." << endl;
            }
        }
        else if (pilihan == 7)
        {
            cout << "Terima kasih telah menggunakan aplikasi kami!" << endl;
        }
        else
        {
            cout << "Pilihan tidak valid!" << endl;
        }
    } while (pilihan != 7);

    return 0;
}
