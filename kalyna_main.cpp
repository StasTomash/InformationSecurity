//
// Created by stast on 10/2/2021.
//

#include "Kalyna/kalyna.h"
#include <iostream>
#include <fstream>
#include <ctime>
#include <string>

using namespace std;

int main() {
    std::vector<WORD_TYPE> key = { 0x0001020304050607ull, 0x08090A0B0C0D0E0Full };
    Kalyna kalyna{};
    if (!kalyna.SetKey(K_128, K_128, key)) {
        return 1;
    }
//    kalyna.LogKalyna();

    ifstream infile;
    infile.open(R"(D:\Study\InformationSecurity\small.txt)", ios::binary | ios::in);

    std::string contents;

    contents.assign(std::istreambuf_iterator<char>(infile),
                    std::istreambuf_iterator<char>());
    infile.close();

    std::vector<BYTE_TYPE> text = {1, 2, 3, 4, 5, 6, 7, 8, 1, 2, 3, 4, 5, 6, 7, 8};

    auto cipher = kalyna.Encipher(text);
    text = kalyna.Decipher(cipher);

    for (auto x : text)
        cout << (int)x << " ";

    return 0;
}