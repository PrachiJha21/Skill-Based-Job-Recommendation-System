#include "utils.h"

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void centerText(string text) {

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    int padding = (columns - text.length()) / 2;

    for (int i = 0; i < padding; i++)
        cout << " ";

    cout << text << endl;
}

void printLine(char symbol) {

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    for (int i = 0; i < columns; i++)
        cout << symbol;

    cout << endl;
}

bool isValidEmail(string email) {

    int atPos = email.find('@');
    int dotPos = email.find('.');

    if (atPos == string::npos)
        return false;

    if (dotPos == string::npos)
        return false;

    if (atPos > dotPos)
        return false;

    if (atPos == 0)
        return false;

    if (dotPos - atPos <= 1)
        return false;

    if (dotPos == email.length() - 1)
        return false;

    return true;
}