#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>
#include "rc4.h"

std::vector<unsigned char> GenerateCharVector(size_t len) {
    std::vector<unsigned char> res(len);
    for (int i = 0; i < len; i++) {
        res[i] = rand() % 256;
    }
    return res;
}

TEST(RC4Test, RC4Test) {
    RC4 rc4;
    std::vector<unsigned char> key, data, cipher;

    key = GenerateCharVector(10);
    data = GenerateCharVector(10);
    rc4.Init(key);
    cipher = rc4.Process(data);
    ASSERT_EQ(data, rc4.Process(cipher));

    key = GenerateCharVector(100);
    data = GenerateCharVector(50000);
    rc4.Init(key);
    cipher = rc4.Process(data);
    ASSERT_EQ(data, rc4.Process(cipher));

    key = GenerateCharVector(256);
    data = GenerateCharVector(100000);
    rc4.Init(key);
    cipher = rc4.Process(data);
    ASSERT_EQ(data, rc4.Process(cipher));
}