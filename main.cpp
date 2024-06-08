#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <filesystem>

using namespace std;
namespace fs = filesystem;

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
        cout << "| " << left << setw(27) << "Masukkan password"
             << " |\n";
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

void registerUser(User &user)
{
    displayInput("username", user.username);
    displayPasswordPrompt(user.password);

    // Check if the output directory exists, if not, create it
    if (!fs::exists("output"))
    {
        fs::create_directory("output");
    }

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

            // Debugging output
            cout << "Membaca dari file: Username=" << dataUsername << ", Password=" << dataPassword << endl;

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
    if (!fs::exists("dataLaguUser"))
    {
        fs::create_directory("dataLaguUser");
    }

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

    if (!found)
    {
        cout << "Judul lagu tidak ditemukan dalam antrian.\n"
             << endl;
        return;
    }

    ofstream outFile(filename);
    if (!outFile.is_open())
    {
        cout << "Tidak dapat membuka file untuk menulis data.\n"
             << endl;
        return;
    }

    for (const auto &lagu : laguList)
    {
        outFile << lagu.judul << ","
                << lagu.artis << ","
                << lagu.genre << "\n";
    }

    outFile.close();
    cout << "Lagu berhasil dihapus dari antrian.\n"
         << endl;
}

Lagu getLaguInfo(const string &judul, const string &filename)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Tidak dapat membuka file " << filename << endl;
        return {"", "", ""};
    }

    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string judulLagu, artis, genre;
        getline(ss, judulLagu, ',');
        getline(ss, artis, ',');
        getline(ss, genre, ',');

        if (judulLagu == judul)
        {
            file.close();
            return {judulLagu, artis, genre};
        }
    }

    file.close();
    return {"", "", ""};
}

// MAIN FUNCTION ===========================================================

void displayMenu()
{
    cout << "          === MUSIKKU ===          " << endl;
    cout << "+----+----------------------------+\n";
    cout << "| No | Menu                       |\n";
    cout << "+----+----------------------------+\n";
    cout << "| 1  | Registrasi                 |\n";
    cout << "+----+----------------------------+\n";
    cout << "| 2  | Login                      |\n";
    cout << "+----+----------------------------+\n";
    cout << "| 3  | Keluar                     |\n";
    cout << "+----+----------------------------+\n";
    cout << "Pilih opsi: ";
}

int main()
{
    int pilihan;
    bool sedangLogin = false;
    User user;

    do
    {
        displayMenu();
        cin >> pilihan;
        cin.ignore(); // to clear the newline character from the input buffer

        if (pilihan == 1)
        {
            registerUser(user);
        }
        else if (pilihan == 2)
        {
            if (loginUser(user))
            {
                sedangLogin = true;

                system("cls");
                createQueue();

                string filename = "dataLaguUser/" + user.username + ".csv"; // Nama file CSV sesuai username

                while (true)
                {
                    system("cls");
                    cout << endl;
                    cout << "        === MUSIKKU ===       " << endl;
                    cout << "+----+------------------------+\n";
                    cout << "| No | Pilihan menu kami      |\n";
                    cout << "+----+------------------------+\n";
                    cout << "| 1  | Tambahkan Lagu         |\n";
                    cout << "+----+------------------------+\n";
                    cout << "| 2  | Hapus Lagu             |\n";
                    cout << "+----+------------------------+\n";
                    cout << "| 3  | Keluar                 |\n";
                    cout << "+----+------------------------+\n";
                    cout << "Pilih opsi: ";

                    string opsiMenu;
                    getline(cin, opsiMenu);

                    if (opsiMenu == "1")
                    {
                        while (true)
                        {
                            system("cls");
                            cout << "     === TAMBAHKAN LAGU ===     " << endl;

                            string judul;
                            cout << "+-----------------------------+\n";
                            cout << "| " << left << setw(27) << ("Masukkan Judul Lagu") << " |\n";
                            cout << "+-----------------------------+\n";
                            cout << "| ";
                            getline(cin, judul);
                            cout << "+-----------------------------+\n";

                            if (judul.empty())
                            {
                                cout << "Input tidak boleh kosong" << endl;
                                system("pause");
                            }
                            else
                            {
                                if (cekJudul(judul, "dataLagu.csv")) // Ganti dengan nama file yang berisi data lagu
                                {
                                    Lagu lagu = getLaguInfo(judul, "dataLagu.csv");
                                    enqueue(lagu);
                                    tambahLaguKeCsv(user.username);
                                }
                                else
                                {
                                    cout << "Judul lagu tidak ditemukan dalam database." << endl;
                                }
                                system("pause");
                                break;
                            }
                        }
                    }
                    else if (opsiMenu == "2")
                    {
                        system("cls");
                        while (true)
                        {
                            ifstream file(filename);

                            cout << "       === HAPUS LAGU ===     \n"
                                 << endl;

                            displayQueue(filename);
                            cout << endl;

                            if (!file.is_open())
                            {
                                system("pause");
                                break;
                            }

                            string judul;
                            cout << "+-----------------------------+\n";
                            cout << "| " << left << setw(27) << ("Masukkan Judul Lagu") << " |\n";
                            cout << "+-----------------------------+\n";
                            cout << "| ";
                            getline(cin, judul);
                            cout << "+-----------------------------+\n";

                            if (judul.empty())
                            {
                                system("pause");
                                break;
                            }
                            else
                            {
                                deleteQueue(judul, filename);
                                system("pause");
                                break;
                            }
                        }
                    }
                    else if (opsiMenu == "3")
                    {
                        break;
                    }
                    else
                    {
                        cout << "Input tidak valid!" << endl;
                    }
                }
            }
        }
        else if (pilihan == 3)
        {
            system("cls");
            cout << "Keluar dari program.\n";
        }
        else
        {
            cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 3);

    return 0;
}
