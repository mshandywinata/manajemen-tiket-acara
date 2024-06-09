#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <stack>
#include <thread>
#include <chrono>

using namespace std;
const string DATA_LAGU = "dataLagu.csv";

struct dataPengguna
{
    string username;
    string password;
};

class User
{
public:
    string username;
    string password;
};

void displayInput(const string &inputan, string &inputValue)
{
    while (true)
    {
        cout << "+-----------------------------+\n";
        cout << "| " << left << setw(27) << ("Masukkan " + inputan) << " |\n";
        cout << "+-----------------------------+\n";
        cout << "| ";
        getline(cin, inputValue);
        cout << "+-----------------------------+\n";

        if (inputValue.empty())
        {
            cout << "Mohon masukkan " << inputan << " terlebih dahulu" << endl;
        }
        else
        {
            break;
        }
    }
}

void displayPasswordPrompt(string &password)
{
    while (true)
    {
        cout << "+-----------------------------+\n";
        cout << "| " << left << setw(27) << "Masukkan password" << " |\n";
        cout << "+-----------------------------+\n";
        cout << "| ";

        password.clear();
        char karakter;
        while ((karakter = _getch()) != '\r')
        {
            if (karakter == '\b')
            {
                if (!password.empty())
                {
                    cout << "\b \b";
                    password.pop_back();
                }
            }
            else
            {
                cout << '*';
                password += karakter;
            }
        }
        cout << "\n+-----------------------------+\n";

        if (password.empty())
        {
            cout << "Mohon masukkan password terlebih dahulu" << endl;
        }
        else
        {
            break;
        }
    }
}

void createDirectory(const string &path)
{
    system(("mkdir " + path).c_str());
}

void registerUser(User &user)
{
    displayInput("username", user.username);
    displayPasswordPrompt(user.password);

    createDirectory("output");

    ofstream file("output/users.csv", ios::app);
    if (file.is_open())
    {
        file << user.username << "," << user.password << endl;
        file.close();
        cout << "\nRegistrasi berhasil" << endl;
    }
    else
    {
        cout << "\nGagal membuka file" << endl;
    }
}

bool loginUser(User &user)
{
    displayInput("username", user.username);
    displayPasswordPrompt(user.password);

    ifstream file("output/users.csv");
    if (file.is_open())
    {
        string line, dataUsername, dataPassword;
        bool dataUserDitemukan = false;
        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, dataUsername, ',');
            getline(ss, dataPassword, ',');

            if (dataUsername == user.username)
            {
                dataUserDitemukan = true;
                if (dataPassword == user.password)
                {
                    file.close();
                    cout << "\nLogin berhasil" << endl;
                    return true;
                }
                else
                {
                    cout << "\nPassword salah, coba periksa kembali" << endl;
                    file.close();
                    return false;
                }
            }
        }
        file.close();
        if (!dataUserDitemukan)
        {
            cout << "\nUsername tidak ditemukan, Anda perlu mendaftar terlebih dahulu." << endl;
        }
    }
    else
    {
        cout << "\nGagal membuka file" << endl;
    }

    return false;
}

// FITUR TAMBAH HAPUS LAGU ===========================================================

const int MAKS_QUEUE = 100;

struct Lagu
{
    string judul;
    string artis;
    string genre;
};

struct Queue
{
    int front, rear;
    Lagu isi[MAKS_QUEUE];
} queue;

void createQueue()
{
    queue.front = queue.rear = -1;
}

bool isEmpty()
{
    return queue.front == -1;
}

bool isFull()
{
    return (queue.rear + 1) % MAKS_QUEUE == queue.front;
}

void enqueue(const Lagu &lagu)
{
    if (isFull())
    {
        cout << "Antrian penuh, lagu tidak dapat ditambahkan." << endl;
    }
    else
    {
        if (isEmpty())
        {
            queue.front = queue.rear = 0;
        }
        else
        {
            queue.rear = (queue.rear + 1) % MAKS_QUEUE;
        }
        queue.isi[queue.rear] = lagu;
    }
}

void dequeue()
{
    if (isEmpty())
    {
        cout << "Antrian kosong, tidak ada lagu yang dihapus." << endl;
    }
    else
    {
        cout << "Menambah lagu: " << queue.isi[queue.front].judul << ", "
             << queue.isi[queue.front].artis << ", "
             << queue.isi[queue.front].genre << endl;
        if (queue.front == queue.rear)
        {
            queue.front = queue.rear = -1;
        }
        else
        {
            queue.front = (queue.front + 1) % MAKS_QUEUE;
        }
    }
}

void tambahLaguKeCsv(const string &username)
{
    string filename = "dataLaguUser/" + username + ".csv";

    // Check if the directory exists, if not, create it
    createDirectory("dataLaguUser");

    ofstream file(filename, ios::app);

    if (!file.is_open())
    {
        cout << "Tidak dapat membuka file untuk menambahkan data." << endl;
        return;
    }

    while (!isEmpty())
    {
        file << queue.isi[queue.front].judul << ","
             << queue.isi[queue.front].artis << ","
             << queue.isi[queue.front].genre << "\n";
        dequeue();
    }

    file.close();
    cout << "Lagu-lagu berhasil ditambahkan ke dalam file CSV untuk pengguna " << username << "\n"
         << endl;
}

bool cekJudul(const string &judul, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Tidak dapat membuka file " << filename << endl;
        return false;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string judulLagu;

        getline(ss, judulLagu, ',');
        if (judulLagu == judul)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
}

void displayQueue(const string &filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Tidak dapat membuka file " << filename << endl;
        return;
    }

    string line;
    int index = 1;
    bool isEmpty = true;
    cout << "Isi Queue:" << endl;
    while (getline(file, line))
    {
        isEmpty = false;
        stringstream ss(line);
        string judul, artis, genre;
        getline(ss, judul, ',');
        getline(ss, artis, ',');
        getline(ss, genre, ',');
        cout << "[" << index++ << "] " << judul << " - " << artis << " - " << genre << endl;
    }

    if (isEmpty)
    {
        cout << "File kosong, tekan enter untuk kembali" << endl;
    }

    file.close();
}

void deleteQueue(const string &judul, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Tidak dapat membuka file " << filename << "\n"
             << endl;
        return;
    }

    vector<Lagu> laguList;
    string line;
    bool found = false;

    while (getline(file, line))
    {
        stringstream ss(line);
        string judulLagu, artis, genre;
        getline(ss, judulLagu, ',');
        getline(ss, artis, ',');
        getline(ss, genre, ',');

        if (judulLagu == judul)
        {
            found = true;
            cout << "Lagu " << judul << " sudah keluar dari queue.\n"
                 << endl;
        }
        else
        {
            laguList.push_back({judulLagu, artis, genre});
        }
    }

    file.close();

    if (found)
    {
        ofstream outFile(filename);
        if (!outFile.is_open())
        {
            cout << "Tidak dapat membuka file " << filename << "\n"
                 << endl;
            return;
        }

        for (const auto &lagu : laguList)
        {
            outFile << lagu.judul << "," << lagu.artis << "," << lagu.genre << endl;
        }

        outFile.close();
    }
    else
    {
        cout << "Lagu dengan judul \"" << judul << "\" tidak ditemukan dalam queue.\n"
             << endl;
    }
}

void tambahLagu()
{
    string judul, artis, genre;

    displayInput("judul lagu", judul);
    displayInput("artis", artis);
    displayInput("genre", genre);

    enqueue({judul, artis, genre});

    cout << "\nLagu berhasil ditambahkan ke queue.\n"
         << endl;
}

void hapusLagu()
{
    string judul;
    displayInput("judul lagu yang ingin dihapus dari queue", judul);

    string filename = "dataLaguUser/" + queue.isi[queue.front].judul + ".csv";
    deleteQueue(judul, filename);
}

// FITUR TAMPILAN MENU ===========================================================

void menampilkanMenu()
{
    cout << "+-----------------------------+\n";
    cout << "|        SELAMAT DATANG       |\n";
    cout << "|          DI APLIKASI        |\n";
    cout << "|           MUSIC APP         |\n";
    cout << "+-----------------------------+\n";
}

void tampilanMenuRegister()
{
    cout << "\n=== MENU REGISTER ===" << endl;
}

void tampilanMenuLogin()
{
    cout << "\n=== MENU LOGIN ===" << endl;
}

void tampilanMenuUtama()
{
    cout << "\n=== MENU UTAMA ===" << endl;
    cout << "1. Tambah Lagu" << endl;
    cout << "2. Hapus Lagu" << endl;
    cout << "3. Tampilkan Queue Lagu" << endl;
    cout << "4. Simpan Lagu ke File" << endl;
    cout << "5. Mainkan Lagu dari Urutan Terakhir" << endl;
    cout << "6. Keluar" << endl;
}

void tampilanMenuTambahLagu()
{
    cout << "\n=== MENU TAMBAH LAGU ===" << endl;
}

void tampilanMenuHapusLagu()
{
    cout << "\n=== MENU HAPUS LAGU ===" << endl;
}

void tampilanMenuTampilkanQueue()
{
    cout << "\n=== MENU TAMPILKAN QUEUE ===" << endl;
}

// PLAYING SONGS FROM STACK =====================================================

stack<Lagu> laguStack;

void loadSongsToStack(const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Tidak dapat membuka file " << filename << endl;
        return;
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string judul, artis, genre;
        getline(ss, judul, ',');
        getline(ss, artis, ',');
        getline(ss, genre, ',');

        laguStack.push({judul, artis, genre});
    }

    file.close();
}

void countdownLagunya(int seconds)
{
    while (seconds > 0)
    {
        cout << "Playing... Next song in " << seconds << " seconds." << endl;
        this_thread::sleep_for(chrono::seconds(1));
        seconds--;
    }
}

void playSongsFromStack()
{
    while (!laguStack.empty())
    {
        Lagu currentLagu = laguStack.top();
        laguStack.pop();

        cout << "Playing: " << currentLagu.judul << " - " << currentLagu.artis << " - " << currentLagu.genre << endl;
        countdownLagunya(15);
    }

    cout << "No more songs in the stack." << endl;
}

// FUNCTION MAIN ===========================================================

int main()
{
    int pilihan;
    User user;
    createQueue();

    while (true)
    {
        menampilkanMenu();
        cout << "1. Register" << endl;
        cout << "2. Login" << endl;
        cout << "3. Exit" << endl;
        cout << "Masukkan pilihan: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan)
        {
        case 1:
            tampilanMenuRegister();
            registerUser(user);
            break;
        case 2:
            tampilanMenuLogin();
            if (loginUser(user))
            {
                while (true)
                {
                    tampilanMenuUtama();
                    cout << "Masukkan pilihan: ";
                    cin >> pilihan;
                    cin.ignore();

                    switch (pilihan)
                    {
                    case 1:
                        tampilanMenuTambahLagu();
                        tambahLagu();
                        break;
                    case 2:
                        tampilanMenuHapusLagu();
                        hapusLagu();
                        break;
                    case 3:
                        tampilanMenuTampilkanQueue();
                        displayQueue("dataLaguUser/" + user.username + ".csv");
                        break;
                    case 4:
                        tambahLaguKeCsv(user.username);
                        break;
                    case 5:
                        loadSongsToStack("dataLaguUser/" + user.username + ".csv");
                        playSongsFromStack();
                        break;
                    case 6:
                        cout << "Terima kasih telah menggunakan aplikasi ini!" << endl;
                        return 0;
                    default:
                        cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
                    }
                }
            }
            break;
        case 3:
            cout << "Terima kasih telah menggunakan aplikasi ini!" << endl;
            return 0;
        default:
            cout << "Pilihan tidak valid. Silakan coba lagi." << endl;
        }
    }

    return 0;
}
