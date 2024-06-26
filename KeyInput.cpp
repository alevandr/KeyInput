#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <streambuf>
#include <map>

struct KeyState
{
    wchar_t KeyValue;
    bool UpperCase;
    void Press();
};

void KeyState::Press()
{
    auto KeyDown = [](WORD wScan) {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_SCANCODE;
        input.ki.wScan = wScan;
        SendInput(1, &input, sizeof(input));
        };
    auto KeyUp = [](WORD wScan) {
        INPUT input = { 0 };
        input.type = INPUT_KEYBOARD;
        input.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
        input.ki.wScan = wScan;
        SendInput(1, &input, sizeof(input));
        };

    SHORT key = 0x0;
    if (VkKeyScan(KeyValue) == -1)
        key = KeyValue;
    else 
        key = VkKeyScan(KeyValue);

    const UINT mappedKey = MapVirtualKey(LOBYTE(key), 0);
    if (UpperCase)
    {
        KeyDown(MapVirtualKey(VK_LSHIFT, 0));
        KeyDown(mappedKey);
        KeyUp(MapVirtualKey(VK_LSHIFT, 0));
    }
    else
    {
        KeyDown(mappedKey);
    }
}

KeyState ConvertChar(wchar_t ch) 
{
    if (isupper(ch))
        return { ch, true };

    std::map<wchar_t, KeyState> converts = {
        {',', {0xbc, false}},
        {'<', {0xbc, true}},
        {'`', {0xc0, false}},
        {'~', {0xc0, true}},
        {';', {0xba, false}},
        {':', {0xba, true}},
        {'[', {0xdb, false}},
        {'{', {0xdb, true}},
        {']', {0xdd, false}},
        {'}', {0xdd, true}},
        {'\'', {0xde, false}},
        {'\"', {0xde, true}},
        {'.', {0xbe, false}},
        {'>', {0xbe, true}},
        {'\n', {0x0d, false}}
    };

    if (converts.find(ch) != converts.end())
        return converts[ch];
    return {ch, false};
}

void SimulateKeyPress(std::vector<KeyState> Keys)
{
    auto ShowLogs = [](KeyState key) 
        {
        std::cout
            << '\n'
            << std::hex << key.KeyValue
            << " " << isupper(key.KeyValue);
        };

    for (auto& key : Keys)
    {
        ShowLogs(key);
        key.Press();
    }
}

std::string getFileText() 
{
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

int main() 
{
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    for (size_t i = 0; i < 5; i++)
    {
        std::cout << i << std::endl;
        Sleep(1000);
    }

    std::string text = getFileText();
    std::vector<KeyState> Keys;

    for (char i : text) 
    {
        Keys.emplace_back(ConvertChar(i));
    }
    SimulateKeyPress(Keys);
    return 0;
}