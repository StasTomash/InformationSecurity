#pragma clang diagnostic push
#pragma ide diagnostic ignored "hicpp-signed-bitwise"
//
// Created by stast on 9/12/2021.
//

#ifndef INFORMATIONSECURITY_AES_H
#define INFORMATIONSECURITY_AES_H

#include <string>
#include <iostream>
#include "constants.h"

class AES {
private:

    const int bytes = 16;
    int Nb = 4; // BLOCK SIZE
    int Nk{}; // KEY LENGTH 4 (128), 6 (192), 8 (256)
    int Nr{}; // NUMBER OF ROUNDS
    unsigned char* round_keys{};

    void AddRoundKey(unsigned char* text, int round) {
        for (int i = 0; i < bytes; ++i) {
            text[i] ^= round_keys[round * bytes + i];
        }
    }

    void KeyExpansion(const unsigned char* key /* 4*key_len elements */,
                      unsigned char* w /* 16*(num_rounds+1) elements */) const {
        unsigned char temp[4];
        for (int i = 0; i < Nk*Nb; ++i) {
            w[i] = key[i];
        }
        for (int i = Nk; i < Nb*(Nr + 1); ++i) {
            for (int j = 0; j < Nb; ++j) {
                temp[j] = w[(i - 1)*Nb + j];
            }
            if (i % Nk == 0) {
                RotWord(temp);
                SubWord(temp);
                temp[0] ^= RCon[i / Nk];
            }
            else if (Nk > 6 && i % Nk == 4) {
                SubWord(temp);
            }
            for (int j = 0; j < Nb; ++j) {
                w[i*Nb + j] = w[(i - Nk)*Nb + j] ^ temp[j];
            }
        }
    }

    static void SubBytes(unsigned char a[16]) {
        for (size_t i = 0; i < 16; ++i) {
            a[i] = S_box[a[i]];
        }
    }


    static void InvSubBytes(unsigned char a[16]) {
        for (size_t i = 0; i < 16; ++i) {
            a[i] = Inv_S_box[a[i]];
        }
    }

    static void ShiftRows(unsigned char a[16]) {
        unsigned char tmp[16];

        tmp[0] = a[0];
        tmp[1] = a[5];
        tmp[2] = a[10];
        tmp[3] = a[15];

        tmp[4] = a[4];
        tmp[5] = a[9];
        tmp[6] = a[14];
        tmp[7] = a[3];

        tmp[8] = a[8];
        tmp[9] = a[13];
        tmp[10] = a[2];
        tmp[11] = a[7];

        tmp[12] = a[12];
        tmp[13] = a[1];
        tmp[14] = a[6];
        tmp[15] = a[11];

        memcpy(a, tmp, 16);
    }

    static void InvShiftRows(unsigned char a[16]) {
        unsigned char tmp[16];

        tmp[0] = a[0];
        tmp[1] = a[13];
        tmp[2] = a[10];
        tmp[3] = a[7];

        tmp[4] = a[4];
        tmp[5] = a[1];
        tmp[6] = a[14];
        tmp[7] = a[11];

        tmp[8] = a[8];
        tmp[9] = a[5];
        tmp[10] = a[2];
        tmp[11] = a[15];

        tmp[12] = a[12];
        tmp[13] = a[9];
        tmp[14] = a[6];
        tmp[15] = a[3];

        memcpy(a, tmp, 16);
    }

    static void MixColumns(unsigned char a[16]) {
        unsigned char tmp[16];
        for (int c = 0; c < 4; ++c) {
            tmp[c * 4] = (unsigned char)(mult2(a[c * 4]) ^ mult3(a[c * 4 + 1]) ^ a[c * 4 + 2] ^ a[c * 4 + 3]);
            tmp[c * 4 + 1] = (unsigned char)(a[c * 4] ^ mult2(a[c * 4 + 1]) ^ mult3(a[c * 4 + 2]) ^ a[c * 4 + 3]);
            tmp[c * 4 + 2] = (unsigned char)(a[c * 4] ^ a[c * 4 + 1] ^ mult2(a[c * 4 + 2]) ^ mult3(a[c * 4 + 3]));
            tmp[c * 4 + 3] = (unsigned char)(mult3(a[c * 4]) ^ a[c * 4 + 1] ^ a[c * 4 + 2] ^ mult2(a[c * 4 + 3]));
        }
        memcpy(a, tmp, 16);
    }

    static void InvMixColumns(unsigned char a[16]) {
        unsigned char tmp[16];
        for (int c = 0; c < 4; ++c) {
            tmp[c * 4] = (unsigned char)(GFMul(0x0e, a[c * 4]) ^ GFMul(0x0b, a[c * 4 + 1]) ^ GFMul(0x0d, a[c * 4 + 2]) ^ GFMul(0x09, a[c * 4 + 3]));
            tmp[c * 4 + 1] = (unsigned char)(GFMul(0x09, a[c * 4]) ^ GFMul(0x0e, a[c * 4 + 1]) ^ GFMul(0x0b, a[c * 4 + 2]) ^ GFMul(0x0d, a[c * 4 + 3]));
            tmp[c * 4 + 2] = (unsigned char)(GFMul(0x0d, a[c * 4]) ^ GFMul(0x09, a[c * 4 + 1]) ^ GFMul(0x0e, a[c * 4 + 2]) ^ GFMul(0x0b, a[c * 4 + 3]));
            tmp[c * 4 + 3] = (unsigned char)(GFMul(0x0b, a[c * 4]) ^ GFMul(0x0d, a[c * 4 + 1]) ^ GFMul(0x09, a[c * 4 + 2]) ^ GFMul(0x0e, a[c * 4 + 3]));
        }
        memcpy(a, tmp, 16);
    }

    static void SubWord(unsigned char a[4]) {
        for (size_t i = 0; i < 4; ++i) {
            a[i] = S_box[a[i]];
        }
    }

    static void RotWord(unsigned char a[4]) {
        unsigned char tmp[4];

        tmp[0] = a[1];
        tmp[1] = a[2];
        tmp[2] = a[3];
        tmp[3] = a[0];

        memcpy(a, tmp, 4);
    }

    static unsigned char mult2(unsigned char p) { // multiply by x
        unsigned char is_high_bit = p & 0x80u;
        unsigned char with_shift = ((unsigned)p << 1u) & 0xffu;
        return is_high_bit == 0 ? with_shift : with_shift ^ 0x1bu;
    }

    static unsigned char mult3(unsigned char p) { // multiply by x+1
        return mult2(p) ^ p;
    }

    static unsigned char GFMul(unsigned char a, unsigned char b) {
        unsigned char p = 0;
        unsigned char hi_bit_set;
        for (int counter = 0; counter < 8; counter++) {
            if ((b & (unsigned char)(1)) != 0) {
                p ^= a;
            }
            hi_bit_set = (unsigned char)(a & (unsigned char)(0x80));
            a <<= 1u;
            if (hi_bit_set != 0) {
                a ^= 0x1bu; /* x^8 + x^4 + x^3 + x + 1 */
            }
            b >>= 1u;
        }
        return p;
    }

    void EncryptInternal(unsigned char* text) {
        AddRoundKey(text, 0);
        for (int i = 1; i < Nr; ++i) {
            SubBytes(text);
            ShiftRows(text);
            MixColumns(text);
            AddRoundKey(text, i);
        }
        SubBytes(text);
        ShiftRows(text);
        AddRoundKey(text, Nr);
    }

    void DecryptInternal(unsigned char* text) {
        AddRoundKey(text, Nr);
        for (int i = 1; i < Nr; ++i) {
            InvShiftRows(text);
            InvSubBytes(text);
            AddRoundKey(text, Nr - i);
            InvMixColumns(text);
        }
        InvShiftRows(text);
        InvSubBytes(text);
        AddRoundKey(text, 0);
    }

    void IncreaseCTRCounter(unsigned char* counter) {
        auto ptr = (unsigned int*)(counter + bytes - 8);
        *ptr = *ptr + 1;
    }

public:
    AES() = default;

    ~AES() {
        delete[] round_keys;
    }

    void SetKey(int key_size, unsigned char* key) {
        Nk = key_size;
        if (Nk == 4) {
            Nr = 10;
        }
        else if (Nk == 6) {
            Nr = 12;
        }
        else if (Nk == 8) {
            Nr = 14;
        }

        delete[] round_keys;
        round_keys = new unsigned char[(Nr + 1)*bytes];
        KeyExpansion(key, round_keys);
    }

    std::string EncryptECB(const std::string& data) {
        size_t textLenFit = ((data.length() + (bytes - 1)) / bytes) * bytes;
        auto textBytes = (unsigned char*)malloc(textLenFit);
        for (int i = 0; i < data.length(); i++) {
            textBytes[i] = data[i];
        }
        for (int i = data.length(); i < textLenFit; i++) {
            textBytes[i] = 0;
        }
        for (int i = 0; i < textLenFit; i += bytes) {
            EncryptInternal(textBytes + i);
        }
        std::string res;
        res.resize(textLenFit);
        for (int i = 0; i < textLenFit; i++) {
            res[i] = textBytes[i];
        }
        return res;
    }

    std::string DecryptECB(std::string data) {
        auto dataBytes = (unsigned char*)malloc(data.size() + 1);
        for (int i = 0; i < data.size(); i++) {
            dataBytes[i] = data[i];
        }
        for (int i = 0; i < data.size(); i += bytes) {
            DecryptInternal(dataBytes + i);
        }
        dataBytes[data.size()] = '\0';
        return std::string((char*)dataBytes);
    }

    std::string EncryptCBC(const std::string& data, const std::string& salt) {
        size_t textLenFit = ((data.length() + (bytes - 1)) / bytes) * bytes;
        auto textBytes = (unsigned char*)malloc(textLenFit);
        auto saltBytes = (unsigned char*)malloc(textLenFit);
        for (int i = 0; i < bytes; i++) {
            saltBytes[i] = salt[i];
        }
        for (int i = 0; i < data.length(); i++) {
            textBytes[i] = data[i];
        }
        for (int i = data.length(); i < textLenFit; i++) {
            textBytes[i] = 0;
        }
        for (int i = 0; i < textLenFit; i += bytes) {
            for (int j = 0; j < bytes; j++) {
                textBytes[i + j] ^= saltBytes[j];
            }
            EncryptInternal(textBytes + i);
            for (int j = 0; j < bytes; j++) {
                saltBytes[j] = textBytes[i + j];
            }
        }
        std::string res;
        res.resize(textLenFit);
        for (int i = 0; i < textLenFit; i++) {
            res[i] = textBytes[i];
        }
        return res;
    }

    std::string DecryptCBC(std::string data, const std::string& salt) {
        auto dataBytes = (unsigned char*)malloc(data.size() + 1);
        auto saltBytes = (unsigned char*)malloc(data.size());
        auto oldSaltBytes = (unsigned char*)malloc(data.size());
        for (int i = 0; i < bytes; i++) {
            saltBytes[i] = salt[i];
        }
        for (int i = 0; i < data.size(); i++) {
            dataBytes[i] = data[i];
        }
        for (int i = 0; i < data.size(); i += bytes) {
            for (int j = 0; j < bytes; j++) {
                oldSaltBytes[j] = saltBytes[j];
                saltBytes[j] = dataBytes[i + j];
            }
            DecryptInternal(dataBytes + i);
            for (int j = 0; j < bytes; j++) {
                dataBytes[i + j] ^= oldSaltBytes[j];
            }
        }
        dataBytes[data.size()] = '\0';
        return std::string((char*)dataBytes);
    }

    std::string EncryptCFB(const std::string& data, const std::string& salt) {
        size_t textLenFit = ((data.length() + (bytes - 1)) / bytes) * bytes;
        auto textBytes = (unsigned char*)malloc(textLenFit);
        auto saltBytes = (unsigned char*)malloc(textLenFit);
        for (int i = 0; i < bytes; i++) {
            saltBytes[i] = salt[i];
        }
        for (int i = 0; i < data.length(); i++) {
            textBytes[i] = data[i];
        }
        for (int i = data.length(); i < textLenFit; i++) {
            textBytes[i] = 0;
        }
        for (int i = 0; i < textLenFit; i += bytes) {
            EncryptInternal(saltBytes);
            for (int j = 0; j < bytes; j++) {
                textBytes[i + j] ^= saltBytes[j];
                saltBytes[j] = textBytes[i + j];
            }
        }
        std::string res;
        res.resize(textLenFit);
        for (int i = 0; i < textLenFit; i++) {
            res[i] = textBytes[i];
        }
        return res;
    }

    std::string DecryptCFB(std::string data, const std::string& salt) {
        auto dataBytes = (unsigned char*)malloc(data.size() + 1);
        auto saltBytes = (unsigned char*)malloc(data.size());
        auto oldSaltBytes = (unsigned char*)malloc(data.size());
        for (int i = 0; i < bytes; i++) {
            saltBytes[i] = salt[i];
        }
        for (int i = 0; i < data.size(); i++) {
            dataBytes[i] = data[i];
        }
        for (int i = 0; i < data.size(); i += bytes) {
            for (int j = 0; j < bytes; j++) {
                oldSaltBytes[j] = saltBytes[j];
                saltBytes[j] = dataBytes[i + j];
            }
            EncryptInternal(oldSaltBytes);
            for (int j = 0; j < bytes; j++) {
                dataBytes[i + j] ^= oldSaltBytes[j];
            }
        }
        dataBytes[data.size()] = '\0';
        return std::string((char*)dataBytes);
    }

    std::string EncryptOFB(const std::string& data, const std::string& salt) {
        size_t textLenFit = ((data.length() + (bytes - 1)) / bytes) * bytes;
        auto textBytes = (unsigned char*)malloc(textLenFit);
        auto saltBytes = (unsigned char*)malloc(textLenFit);
        for (int i = 0; i < bytes; i++) {
            saltBytes[i] = salt[i];
        }
        for (int i = 0; i < data.length(); i++) {
            textBytes[i] = data[i];
        }
        for (int i = data.length(); i < textLenFit; i++) {
            textBytes[i] = 0;
        }
        for (int i = 0; i < textLenFit; i += bytes) {
            EncryptInternal(saltBytes);
            for (int j = 0; j < bytes; j++) {
                textBytes[i + j] ^= saltBytes[j];
            }
        }
        std::string res;
        res.resize(textLenFit);
        for (int i = 0; i < textLenFit; i++) {
            res[i] = textBytes[i];
        }
        return res;
    }

    std::string DecryptOFB(std::string data, const std::string& salt) {
        auto dataBytes = (unsigned char*)malloc(data.size() + 1);
        auto saltBytes = (unsigned char*)malloc(data.size());
        for (int i = 0; i < bytes; i++) {
            saltBytes[i] = salt[i];
        }
        for (int i = 0; i < data.size(); i++) {
            dataBytes[i] = data[i];
        }
        for (int i = 0; i < data.size(); i += bytes) {
            EncryptInternal(saltBytes);
            for (int j = 0; j < bytes; j++) {
                dataBytes[i + j] ^= saltBytes[j];
            }
        }
        dataBytes[data.size()] = '\0';
        return std::string((char*)dataBytes);
    }

    std::string EncryptCTR(const std::string& data, const std::string& salt) {
        size_t textLenFit = ((data.length() + (bytes - 1)) / bytes) * bytes;
        auto textBytes = (unsigned char*)malloc(textLenFit);
        auto saltBytes = (unsigned char*)malloc(textLenFit);
        for (int i = 0; i < bytes - 8; i++) {
            saltBytes[i] = salt[i];
        }
        for (int i = bytes - 8; i < bytes; i++) {
            saltBytes[i] = 0;
        }
        for (int i = 0; i < data.length(); i++) {
            textBytes[i] = data[i];
        }
        for (int i = data.length(); i < textLenFit; i++) {
            textBytes[i] = 0;
        }
        for (int i = 0; i < textLenFit; i += bytes) {
            EncryptInternal(saltBytes);
            IncreaseCTRCounter(saltBytes);
            for (int j = 0; j < bytes; j++) {
                textBytes[i + j] ^= saltBytes[j];
            }
        }
        std::string res;
        res.resize(textLenFit);
        for (int i = 0; i < textLenFit; i++) {
            res[i] = textBytes[i];
        }
        return res;
    }

    std::string DecryptCTR(std::string data, const std::string& salt) {
        auto dataBytes = (unsigned char*)malloc(data.size() + 1);
        auto saltBytes = (unsigned char*)malloc(data.size());

        for (int i = 0; i < bytes - 8; i++) {
            saltBytes[i] = salt[i];
        }
        for (int i = bytes - 8; i < bytes; i++) {
            saltBytes[i] = 0;
        }
        for (int i = 0; i < data.size(); i++) {
            dataBytes[i] = data[i];
        }
        for (int i = 0; i < data.size(); i += bytes) {
            EncryptInternal(saltBytes);
            IncreaseCTRCounter(saltBytes);
            for (int j = 0; j < bytes; j++) {
                dataBytes[i + j] ^= saltBytes[j];
            }
        }
        dataBytes[data.size()] = '\0';
        return std::string((char*)dataBytes);
    }
};

#endif //INFORMATIONSECURITY_AES_H
