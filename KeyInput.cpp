#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>
#include <map>

wchar_t ConvertChar(wchar_t ch) 
{
    wchar_t result;
    std::map<wchar_t, wchar_t> converts = {
        {',', 0xbc},
        {'`', 0xc0},
        {';', 0xba},
        {'[', 0xdb},
        {']', 0xdd},
        {'\'', 0xde},
        {'.', 0xbe},
        {'\n', 0x0d}
    };
    if (converts.find(ch) != converts.end())
        return converts[ch];
    return ch;
}

void PressButtonWithShift(wchar_t character)
{
    INPUT input = { 0 };
    const SHORT key = VkKeyScan(character + 0x20);
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

void PressButton(wchar_t character) 
{
    INPUT input = { 0 };
    SHORT key = 0x0;
    if (character == 0x20)
        key = 0x20;
    else
    {
        if (VkKeyScan(character - 0x20) == -1)
            key = character;
        else
            key = VkKeyScan(character);
    }
    const UINT mappedKey = MapVirtualKey(LOBYTE(key), 0);

    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_SCANCODE;
    input.ki.wScan = mappedKey;
    SendInput(1, &input, sizeof(input));
}

void SimulateKeyPress(std::vector<wchar_t> v) 
{
    for (int i = 0; i < v.size(); i++) 
    {
        std::cout 
            << '\n' 
            << std::hex << v[i] 
            << " " << ConvertChar(v[i]) 
            << " " << isupper(v[i]);

        v[i] = ConvertChar(v[i]);

        if (isupper(v[i]))
        {
            PressButtonWithShift(v[i]);
        }
        else 
        {
            PressButton(v[i]);
        }
        //Sleep(1);
    }
}

std::string getFileText() {

    std::ifstream inputFile("input.txt");
    std::string str;
    std::string file_contents;

    while (std::getline(inputFile, str)) 
    {
        file_contents += str;
        file_contents.push_back('\n');
    }
    return file_contents;
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
    std::cout << "\nhx ks Upper\n";
    for (char i : text) 
    {
        v.emplace_back(i);
    }
    SimulateKeyPress(v);
    std::cout << "\n" << "==============\n";
    return 0;
}