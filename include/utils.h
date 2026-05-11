#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <windows.h>
#include <string>

using namespace std;

extern HANDLE h;

void centerText(string text);

void printLine(char symbol = '=');

bool isValidEmail(string email);

#endif