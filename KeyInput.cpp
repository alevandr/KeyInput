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
    INPUT input = { 0 };
    for (int i = 0; i < v.size(); i++) {

        std::cout << '\n' << std::hex << v[i] << " " << isupper(v[i]);
        if (isupper(v[i]))
        {

            const SHORT key = VkKeyScan(v[i] + 0x20);
            const UINT mappedKey = MapVirtualKey(LOBYTE(key), 0);

            input.type = INPUT_KEYBOARD;
            input.ki.dwFlags = KEYEVENTF_SCANCODE;
            input.ki.wScan = MapVirtualKey(VK_LSHIFT, 0);
            SendInput(1, &input, sizeof(input));

            input.type = INPUT_KEYBOARD;
            input.ki.dwFlags = KEYEVENTF_SCANCODE;
            input.ki.wScan = mappedKey;
            SendInput(1, &input, sizeof(input));

            input.type = INPUT_KEYBOARD;
            input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
            input.ki.wScan = mappedKey;
            SendInput(1, &input, sizeof(input));

            input.type = INPUT_KEYBOARD;
            input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
            input.ki.wScan = MapVirtualKey(VK_LSHIFT, 0);
            SendInput(1, &input, sizeof(input));
        }
        else 
        {
            const SHORT key = v[i] == 0x20 ? 0x20 : VkKeyScan(v[i] - 0x20);
            const UINT mappedKey = MapVirtualKey(LOBYTE(key), 0);

            input.type = INPUT_KEYBOARD;
            input.ki.dwFlags = KEYEVENTF_SCANCODE;
            input.ki.wScan = mappedKey;
            SendInput(1, &input, sizeof(input));
        }
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
    for (size_t i = 0; i < 5; i++)
    {
        std::cout << i << std::endl;
        Sleep(1000);
    }
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