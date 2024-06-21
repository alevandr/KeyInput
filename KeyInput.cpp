#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <sstream> 
#include <fstream>
#include <streambuf>
#include <thread>
#include <chrono>
#include <tchar.h>
#include <map>
#include <random>
#include <ctime>
#include <cstdlib>

std::string getFileText() {
    std::ifstream t("input.txt");
    std::string str;
    std::string file_contents;
    while (std::getline(t, str)) {
        file_contents += str;
        file_contents.push_back('\n');
    }
    return file_contents;
}

void SimulateKeyPress(std::vector<wchar_t> v) {
    byte simb;
    HKL kbl = GetKeyboardLayout(0);
    for (int i = 0; i < v.size(); i++) {
        std::cout << '\n' << std::hex << (int)v[i] - 0x20;
        INPUT inp;
        inp.type = INPUT_KEYBOARD;
        inp.ki.time = 0;
        inp.ki.dwExtraInfo = 0;
        inp.ki.dwFlags = KEYEVENTF_UNICODE;
        inp.ki.wScan = v[i] - 0x20;
        inp.ki.wVk = VkKeyScanEx(v[i] - 0x20, kbl);
        if (v[i] == 0x20)
            inp.ki.wVk = 0x20;
        SendInput(1, &inp, sizeof(inp));
        Sleep(1);
    }
}

wchar_t check_special_char(wchar_t ch) {
    wchar_t result;
    switch (ch) {
    case ',':
        result = 0xbc;
    case '`':
        result = 0xc0;
    case ';':
        result = 0xba;
    case '[':
        result = 0xdb;
    case ']':
        result = 0xdd;
    case '\'':
        result = 0xde;
    case '.':
        result = 0xbe;
    case '\n':
        result = 0x0d;
    default:
        return ch;
    }
    return result + 0x20;
}

int main() {
    Sleep(5000);
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);

    std::string text = getFileText();
    std::vector<wchar_t> v;
    
    for (char i : text) 
    {
        v.push_back(i);
    }
    SimulateKeyPress(v);
    std::cout << "\n" << "==============\n";
    //HKL kb = GetKeyboardLayout(0);
    //std::cout << '\n' << kb;
    return 0;
}