#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>
#include "salsa20.h"

std::vector<unsigned char> GenerateCharVector(size_t len) {
    std::vector<unsigned char> res(len);
    for (int i = 0; i < len; i++) {
        res[i] = rand() % 256;
    }
    return res;
}

std::vector<unsigned int> GenerateWordVector(size_t len) {
    std::vector<unsigned int> res(len);
    for (int i = 0; i < len; i++) {
        res[i] = (unsigned)rand() | ((unsigned)rand() << 16u);
    }
    return res;
}

TEST(Salsa20Test, RC4Test) {
    Salsa20 salsa20;
    std::vector<unsigned char> data, cipher;
    std::vector<unsigned int> key;

    key = GenerateWordVector(8);
    data = GenerateCharVector(10);
    salsa20.SetKey(key);
    cipher = salsa20.Process(data);
    ASSERT_EQ(data, salsa20.Process(cipher));

    data = GenerateCharVector(2560);
    salsa20.SetKey(key);
    cipher = salsa20.Process(data);
    ASSERT_EQ(data, salsa20.Process(cipher));

    key = GenerateWordVector(8);
    data = GenerateCharVector(500000);
    salsa20.SetKey(key);
    cipher = salsa20.Process(data);
    ASSERT_EQ(data, salsa20.Process(cipher));
}