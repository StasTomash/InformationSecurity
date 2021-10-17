#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>
#include "AES.h"

void GenerateKey(unsigned char* key, size_t keyLen) {
    for (int i = 0; i < keyLen; i++) {
        key[i] = rand() % 255 + 1;
    }
}

std::string GenerateString(size_t len) {
    std::string res;
    res.resize(len);
    for (int i = 0; i < len; i++) {
        res[i] = rand() % 254 + 1;
    }
    return res;
}

TEST(AESTest, ECBTest) {
    AES aes4;
    auto* key = (unsigned char*)malloc(16);
    std::string data, cipher;

    GenerateKey(key, 16);
    data = GenerateString(200);
    aes4.SetKey(4, key);

    cipher = aes4.EncryptECB(data);
    ASSERT_EQ(data, aes4.DecryptECB(cipher));

    data = GenerateString(100000);

    cipher = aes4.EncryptECB(data);
    ASSERT_EQ(data, aes4.DecryptECB(cipher));
}

TEST(AESTest, CBCTest) {
    AES aes4;
    auto* key = (unsigned char*)malloc(16);
    std::string data, cipher, salt;

    GenerateKey(key, 16);
    salt = GenerateString(16);

    data = GenerateString(200);
    aes4.SetKey(4, key);

    cipher = aes4.EncryptCBC(data, salt);
    ASSERT_EQ(data, aes4.DecryptCBC(cipher, salt));

    data = GenerateString(100000);

    cipher = aes4.EncryptCBC(data, salt);
    ASSERT_EQ(data, aes4.DecryptCBC(cipher, salt));
}

TEST(AESTest, CFBTest) {
    AES aes4;
    auto* key = (unsigned char*)malloc(16);
    std::string data, cipher, salt;

    GenerateKey(key, 16);
    salt = GenerateString(16);

    data = GenerateString(200);
    aes4.SetKey(4, key);

    cipher = aes4.EncryptCFB(data, salt);
    ASSERT_EQ(data, aes4.DecryptCFB(cipher, salt));

    data = GenerateString(100000);

    cipher = aes4.EncryptCFB(data, salt);
    ASSERT_EQ(data, aes4.DecryptCFB(cipher, salt));
}

TEST(AESTest, OFBTest) {
    AES aes4;
    auto* key = (unsigned char*)malloc(16);
    std::string data, cipher, salt;

    GenerateKey(key, 16);
    salt = GenerateString(16);

    data = GenerateString(200);
    aes4.SetKey(4, key);

    cipher = aes4.EncryptOFB(data, salt);
    ASSERT_EQ(data, aes4.DecryptOFB(cipher, salt));

    data = GenerateString(100000);

    cipher = aes4.EncryptOFB(data, salt);
    ASSERT_EQ(data, aes4.DecryptOFB(cipher, salt));
}

TEST(AESTest, CTRTest) {
    AES aes4;
    auto* key = (unsigned char*)malloc(16);
    std::string data, cipher, salt;

    GenerateKey(key, 16);
    salt = GenerateString(8);

    data = GenerateString(200);
    aes4.SetKey(4, key);

    cipher = aes4.EncryptCTR(data, salt);
    ASSERT_EQ(data, aes4.DecryptCTR(cipher, salt));

    data = GenerateString(100000);

    cipher = aes4.EncryptCTR(data, salt);
    ASSERT_EQ(data, aes4.DecryptCTR(cipher, salt));
}