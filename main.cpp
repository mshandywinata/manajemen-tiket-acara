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
        system("pause");
    }
    else
    {
        cout << "\nGagal membuka file" << endl;
        system("pause");
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

            // // Debugging output
            // cout << "Membaca dari file: Username=" << dataUsername << ", Password=" << dataPassword << endl;

            if (dataUsername == user.username)
            {
                dataUserDitemukan = true;
                if (dataPassword == user.password)
                {
                    file.close();
                    cout << "\nLogin berhasil" << endl;
                    system("pause");
                    return true;
                }
                else
                {
                    cout << "\nPassword salah, coba periksa kembali" << endl;
                    system("pause");
                    file.close();
                    return false;
                }
            }
        }
        file.close();
        if (!dataUserDitemukan)
        {
            cout << "\nUsername tidak ditemukan, Anda perlu mendaftar terlebih dahulu." << endl;
            system("pause");
        }
    }
    else
    {
        cout << "\nGagal membuka file" << endl;
        cout << "Belum ada user yang terdaftar" << endl;
        system("pause");
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
        Lagu currentLagu = queue.isi[queue.front];
        string judulLagu = currentLagu.judul;

        // Cek apakah judul lagu sudah ada di file CSV
        if (cekJudul(judulLagu, filename))
        {
            cout << "Lagu dengan judul \"" << judulLagu << "\" sudah ada dalam data." << endl;
        }
        else
        {
            file << currentLagu.judul << ","
                 << currentLagu.artis << ","
                 << currentLagu.genre << "\n";
            cout << "Menambahkan lagu: " << currentLagu.judul << ", "
                 << currentLagu.artis << ", "
                 << currentLagu.genre << endl;
        }
        queue.front = queue.rear = -1;
    }

    file.close();
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

bool findQueue(const string &target, const string &filename)
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
        string judul, artis, genre;
        getline(ss, judul, ',');
        getline(ss, artis, ',');
        getline(ss, genre, ',');

        if (judul == target)
        {
            file.close();
            return true;
        }
    }

    file.close();
    return false;
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

// GRAF & DFS UNTUK REKOMENDASI LAGU
int vertex;
vector<vector<int>> adj;
vector<Lagu> daftarLagu;
vector<bool> visited;

void buatGraph(int v)
{
    vertex = v;
    adj.resize(v);
    visited.resize(v, false);
}

void tambahEdge(int vAsal, int vTujuan)
{
    adj[vAsal].push_back(vTujuan);
    adj[vTujuan].push_back(vAsal);
}

void dfs(int start, vector<Lagu> &rekomendasi, const vector<string> &artisQueue, const vector<string> &genreQueue, const string &filename)
{
    visited[start] = true;
    for (int neighbor : adj[start])
    {
        if (!visited[neighbor])
        {
            if (!findQueue(daftarLagu[neighbor].judul, filename) &&
                (find(artisQueue.begin(), artisQueue.end(), daftarLagu[neighbor].artis) != artisQueue.end() ||
                 find(genreQueue.begin(), genreQueue.end(), daftarLagu[neighbor].genre) != genreQueue.end()))
            {
                rekomendasi.push_back(daftarLagu[neighbor]);
            }
            dfs(neighbor, rekomendasi, artisQueue, genreQueue, filename);
        }
    }
}

int cariItem(const string &item)
{
    for (size_t i = 0; i < daftarLagu.size(); ++i)
    {
        if (daftarLagu[i].judul == item)
        {
            return i;
        }
    }
    return -1;
}

void buatGraphCsv()
{
    ifstream file(DATA_LAGU);

    if (!file.is_open())
    {
        cout << "Unable to open file";
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

        daftarLagu.push_back({judul, artis, genre});
    }

    buatGraph(daftarLagu.size());

    // Membuat graf berdasarkan kesamaan artis atau genre
    for (size_t i = 0; i < daftarLagu.size(); ++i)
    {
        for (size_t j = i + 1; j < daftarLagu.size(); ++j)
        {
            if (daftarLagu[i].artis == daftarLagu[j].artis || daftarLagu[i].genre == daftarLagu[j].genre)
            {
                tambahEdge(i, j);
            }
        }
    }

    file.close();
}

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
    string pilihan;
    bool sedangLogin = false;
    User user;
    buatGraphCsv();

    do
    {
        system("cls");
        displayMenu();
        cin >> pilihan;
        cin.ignore(); // to clear the newline character from the input buffer

        if (pilihan == "1")
        {
            registerUser(user);
        }
        else if (pilihan == "2")
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
                    cout << "| 3  | Hasil Rekomendasi      |\n";
                    cout << "+----+------------------------+\n";
                    cout << "| 4  | Keluar                 |\n";
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
                                if (cekJudul(judul, DATA_LAGU)) // Ganti dengan nama file yang berisi data lagu
                                {
                                    Lagu lagu = getLaguInfo(judul, DATA_LAGU);
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
                        system("cls");
                        cout << "       === HASIL REKOMENDASI ===     \n"<< endl;
                        vector<Lagu> daftarRekomendasi;
                        vector<string> artisQueue;
                        vector<string> genreQueue;

                        ifstream file(filename);
                        if (!file.is_open()) {
                            cout << "Tidak bisa menampilkan rekomendasi\nBelum ada lagu yang ditambahkan" << endl;
                            system("pause");
                            continue;
                        }

                        string line;
                        while (getline(file, line))
                        {
                            stringstream ss(line);
                            string judul, artis, genre;
                            getline(ss, judul, ',');
                            getline(ss, artis, ',');
                            getline(ss, genre, ',');

                            artisQueue.push_back(artis);
                            genreQueue.push_back(genre);

                            int songIndex = cariItem(judul);
                            if (songIndex != -1)
                            {
                                fill(visited.begin(), visited.end(), false);
                                dfs(songIndex, daftarRekomendasi, artisQueue, genreQueue, filename);
                            }
                        }
                        file.close();

                        // mengurutkan sesuai abjad & hapus duplikasi
                        sort(daftarRekomendasi.begin(), daftarRekomendasi.end(), [](const Lagu &a, const Lagu &b) {
                            return a.judul < b.judul;
                        });
                        daftarRekomendasi.erase(unique(daftarRekomendasi.begin(), daftarRekomendasi.end(), [](const Lagu &a, const Lagu &b) {
                            return a.judul == b.judul;
                        }), daftarRekomendasi.end());

                        // tampilan rekomendasi dalam batch
                        int totalRekomendasi = daftarRekomendasi.size();
                        int batchSize = 10;
                        int start = 0;

                        if (totalRekomendasi <= 0) {
                            cout << "Tidak bisa menampilkan rekomendasi\nBelum ada lagu yang ditambahkan" << endl;
                            system("pause");
                            continue;
                        }

                        while (start < totalRekomendasi)
                        {
                            system("cls");
                            cout << "       === HASIL REKOMENDASI ===     \n"
                                 << endl;
                            cout << "========== Menampilkan rekomendasi dari " << start + 1 << " hingga " << min(start + batchSize, totalRekomendasi) << " dari total " << totalRekomendasi << " rekomendasi. ==========" << endl;

                            for (int i = start; i < start + batchSize && i < totalRekomendasi; ++i)
                            {
                                cout << "[" << i + 1 << "] " << daftarRekomendasi[i].judul << " - " << daftarRekomendasi[i].artis << " - " << daftarRekomendasi[i].genre << endl;
                            }

                            if (start + batchSize >= totalRekomendasi)
                            {
                                system("pause");
                                break;
                            }

                            cout << "\nIngin melihat 10 rekomendasi lagu selanjutnya? (y/t)\n> ";
                            string jawaban;
                            cin >> jawaban;
                            if (jawaban[0] == 'y')
                            {
                                start += batchSize;
                            }
                            else
                            {
                                break;
                            }
                        }
                    }
                    else if (opsiMenu == "4")
                    {
                        break;
                    }
                    else
                    {
                        cout << "Input tidak valid!" << endl;
                        system("cls");
                    }
                }
            }
        }
        else if (pilihan == "3")
        {
            system("cls");
            cout << "Keluar dari program.\n";
            system("pause");
            system("cls");
        }
        else
        {
            cout << "Pilihan tidak valid.\n";
            system("pause");
        }
    } while (pilihan != "3");

    return 0;
}