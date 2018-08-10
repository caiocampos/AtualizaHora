/* 
 * File:   main.cpp
 * Author: Caio Campos
 *
 * Created on 28 de Maio de 2018, 14:40
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <ctime>
#include <windows.h>

#define DEFAULT_YEAR 2018

using namespace std;

bool initw32time();
bool runResync();
bool verifyYear(int year);
void printExec(LPCSTR s);
void printExec(LPCSTR prog, LPCSTR cmd);

int main(int argc, char** argv)
{
    freopen("log.txt", "w", stdout);
    ifstream yearFile("year.txt");
    int i = 0;
    int year = 0;

    if (not (yearFile >> year))
    {
        year = DEFAULT_YEAR;
        cout << "Usando o ano padrão!" << endl;
    }
    cout << "Ano " << year << endl;

    while (i < 20)
    {
        cout << endl << "Tentativa " << ++i << endl;
        if (initw32time())
        {
            continue;
        }
        if (runResync())
        {
            continue;
        }
        if (verifyYear(year))
        {
            cout << "O ano está válido!" << endl;
            break;
        }
        else
        {
            cout << "O ano ainda não está válido!" << endl;
        }
    }
    cout << "Finalizando w32time" << endl;
    printExec("net", "stop w32time");
    cout << "w32time finalizado" << endl;
    return 0;
}

/**
 * 
 * @return true se ocorrer erro
 */
bool initw32time()
{
    cout << "Iniciando w32time" << endl;
    errno = 0;
    printExec("net", "start w32time");
    Sleep(1000);
    if (errno != 0)
    {
        cout << strerror(errno) << endl;
        cout << "Tentando finalizar w32time" << endl;
        printExec("net", "stop w32time");
        Sleep(1000);
        return true;
    }
    return false;
}

/**
 * 
 * @return true se ocorrer erro
 */
bool runResync()
{
    cout << "Sincronizando horário" << endl;
    errno = 0;
    printExec("w32tm", "/resync /force");
    Sleep(1000);
    if (errno != 0)
    {
        cout << strerror(errno) << endl;
        Sleep(8000);
        return true;
    }
    return false;
}

/**
 * 
 * @return true se ano válido
 */
bool verifyYear(int year)
{
    time_t t = time(NULL);
    tm* timePtr = localtime(&t);
    return (timePtr->tm_year + 1900) >= year;
}

void printExec(LPCSTR cmd)
{
    printExec("cmd.exe", cmd);
}

void printExec(LPCSTR prog, LPCSTR cmd)
{
    cout << "Retorno: " << (int) ShellExecute(NULL, "open", prog, cmd, NULL, SW_HIDE) << endl;
}