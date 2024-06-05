#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <iomanip> // for std::setw

using namespace std;

struct dataPengguna
{
    string username;
    string password;
};

void displayInput(const string &inputan, string &inputValue)
{
    cout << "+-----------------------------+\n";
    cout << "| " << left << setw(27) << ("Masukkan " + inputan) << " |\n";
    cout << "+-----------------------------+\n";
    cout << "| ";
    cin >> inputValue;
    cout << "+-----------------------------+\n";
}

void displayPasswordPrompt(string &password)
{
    cout << "+-----------------------------+\n";
    cout << "| " << left << setw(27) << "Masukkan password" << " |\n";
    cout << "+-----------------------------+\n";
    cout << "| ";

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
}

void registerUser()
{
    string username, password;

    displayInput("username", username);
    displayPasswordPrompt(password);

    ofstream file("users.csv", ios::app);
    if (file.is_open())
    {
        file << username << "," << password << endl;
        file.close();
        cout << "\nRegistrasi berhasil" << endl;
    }
    else
    {
        cout << "\nGagal membuka file" << endl;
    }
}

bool loginUser()
{
    string username, password;

    displayInput("username", username);
    displayPasswordPrompt(password);

    ifstream file("users.csv");
    if (file.is_open())
    {
        string line, dataUsername, dataPassword;
        bool dataUserDitemukan = false;
        while (getline(file, line))
        {
            stringstream ss(line);
            getline(ss, dataUsername, ',');
            getline(ss, dataPassword, ',');

            if (dataUsername == username)
            {
                dataUserDitemukan = true;
                if (dataPassword == password)
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

void displayMenu()
{
    cout << "\n=== Selamat Datang di Aplikasi ===\n";
    cout << "+----+------------------------+\n";
    cout << "| No | Pilihan menu kami      |\n";
    cout << "+----+------------------------+\n";
    cout << "| 1  | Daftar                 |\n";
    cout << "+----+------------------------+\n";
    cout << "| 2  | Login                  |\n";
    cout << "+----+------------------------+\n";
    cout << "| 3  | Keluar                 |\n";
    cout << "+----+------------------------+\n";
    cout << "Pilih opsi: ";
}

int main()
{
    int pilihan;
    bool sedangLogin = false;

    do
    {
        displayMenu();
        cin >> pilihan;

        if (pilihan == 1)
        {
            registerUser();
        }
        else if (pilihan == 2)
        {
            if (loginUser())
            {
                sedangLogin = true;
            }
        }
        else if (pilihan == 3)
        {
            cout << "Keluar dari program.\n";
        }
        else
        {
            cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 3);

    return 0;
}