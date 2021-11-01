//
// Created by stast on 10/31/2021.
//

#ifndef INFORMATIONSECURITY_SHA256_H
#define INFORMATIONSECURITY_SHA256_H

#include "constants.h"
#include <vector>
#include <string>
#include <cassert>
#include <bitset>


unsigned int RotateRight(unsigned int x, size_t bits) {
    unsigned int rotPart = x & ((1u << bits) - 1);
    return (x >> bits) | (rotPart << (32 - bits));
}

class SHA256 {
private:
    unsigned int h[8], a[8];
    void InitH() {
        for (int i = 0; i < 8; i++) {
            h[i] = initialH[i];
        }
    }
    void InitA() {
        for (int i = 0; i < 8; i++) {
            a[i] = h[i];
        }
    }
    template<typename T> std::vector<unsigned char> ConvertToChar(std::vector<T> data) {
        int typeLen = sizeof(T);
        std::vector<unsigned char> charData;
        for (auto item : data) {
            unsigned char* charItem = reinterpret_cast<unsigned char*>(&item);
            for (int i = 0; i < typeLen; i++) {
                charData.push_back(charItem[typeLen - i - 1]);
            }
        }
        return charData;
    }
    static void PadCharData(std::vector<unsigned char>& data) {
        unsigned char nextChar = (unsigned char)(1u << 7u);
        size_t bitLen = data.size() * 8;
        size_t zeroBits = 512 - (bitLen + 65) % 512;
        assert(zeroBits >= 7);
        zeroBits -= 7;
        data.push_back(nextChar);
        while (zeroBits > 0) {
            data.push_back(0);
            zeroBits -= 8;
        }
        unsigned long long bitLen64 = bitLen;
        unsigned char* bitLenChar = reinterpret_cast<unsigned char*>(&bitLen64);
        for (int i = 0; i < 8; i++) {
            data.push_back(bitLenChar[7 - i]);
        }
    }
    static std::vector<unsigned int> ConvertToWords(std::vector<unsigned char> data) {
        std::vector<unsigned int> words;
        for (int i = 0; i < data.size(); i += 4) {
            words.push_back((unsigned int)data[i] << 24u | (unsigned int)data[i + 1] << 16u |
                            (unsigned int)data[i + 2] << 8u | (unsigned int)data[i + 3]);
        }
        return words;
    }
    void ProcessBlock(std::vector<unsigned int> block) {
        for (int i = 16; i < 64; i++) {
            block.push_back(
                block[i - 16] +
                (RotateRight(block[i - 15], 7) ^ RotateRight(block[i - 15], 18) ^ (block[i - 15] >> 3u)) +
                block[i - 7] +
                (RotateRight(block[i - 2], 17) ^ RotateRight(block[i - 2], 19) ^ (block[i - 2] >> 10u))
            );
        }
        InitA();
        for (int i = 0; i < 64; i++) {
            unsigned int sigma0 = RotateRight(a[0], 2) ^ RotateRight(a[0], 13) ^ RotateRight(a[0], 22);
            unsigned int ma = (a[0] & a[1]) ^ (a[0] & a[2]) ^ (a[1] & a[2]);
            unsigned int t2 = sigma0 + ma;
            unsigned int sigma1 = RotateRight(a[4], 6) ^ RotateRight(a[4], 11) ^ RotateRight(a[4], 25);
            unsigned int ch = (a[4] & a[5]) ^ ((~a[4]) & a[6]);
            unsigned int t1 = a[7] + sigma1 + ch + k[i] + block[i];

            a[7] = a[6];
            a[6] = a[5];
            a[5] = a[4];
            a[4] = a[3] + t1;
            a[3] = a[2];
            a[2] = a[1];
            a[1] = a[0];
            a[0] = t1 + t2;
        }

        for (int i = 0; i < 8; i++) {
            h[i] += a[i];
        }
    }
public:
    template<typename T> std::vector<unsigned int> GetHash(std::vector<T> data) {
        InitH();
        auto convertedData = ConvertToChar(data);
        PadCharData(convertedData);
        auto words = ConvertToWords(convertedData);

        for (int blockNum = 0; blockNum < words.size() / 16; blockNum++) {
            std::vector<unsigned int> block(words.begin() + blockNum * 16, words.begin() + (blockNum + 1) * 16);
            ProcessBlock(block);
        }

        return std::vector<unsigned int>(h, h + 8);
    }
};

#endif //INFORMATIONSECURITY_SHA256_H
