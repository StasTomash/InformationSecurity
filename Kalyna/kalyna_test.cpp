#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include "kalyna.h"

class KalynaTest : public ::testing::Test {
protected:
    void SetUp() override {
        key128 = std::vector<WORD_TYPE>( { 0x0001020304050607ull, 0x08090A0B0C0D0E0Full });
        key256 = std::vector<WORD_TYPE>( { 0x0001020304050607ull, 0x08090A0B0C0D0E0Full,
                                              0x0001020304050607ull, 0x08090A0B0C0D0E0Full });
        key512 = std::vector<WORD_TYPE>({ 0x0001020304050607ull, 0x08090A0B0C0D0E0Full,
                                             0x0001020304050607ull, 0x08090A0B0C0D0E0Full,
                                             0x0001020304050607ull, 0x08090A0B0C0D0E0Full,
                                             0x0001020304050607ull, 0x08090A0B0C0D0E0Full, });
        kalyna128_128.Configure(K_128, K_128);
        kalyna128_128.SetKey(key128);
        kalyna128_256.Configure(K_256, K_128);
        kalyna128_256.SetKey(key256);
        kalyna256_256.Configure(K_256, K_256);
        kalyna256_256.SetKey(key256);
        kalyna256_512.Configure(K_512, K_256);
        kalyna256_512.SetKey(key512);
        kalyna512_512.Configure(K_512, K_512);
        kalyna512_512.SetKey(key512);

        smallText = GenerateText(16);
        mediumText = GenerateText(512);
        largeText = GenerateText(16384);
    }

    static std::vector<BYTE_TYPE> GenerateText(size_t len) {
        std::vector<BYTE_TYPE> text(len);
        for (auto& x: text) {
            x = rand() % 255 + 1;
        }
        return text;
    }

    Kalyna kalyna128_128;
    Kalyna kalyna128_256;
    Kalyna kalyna256_256;
    Kalyna kalyna256_512;
    Kalyna kalyna512_512;

    std::vector<WORD_TYPE> key128;
    std::vector<WORD_TYPE> key256;
    std::vector<WORD_TYPE> key512;

    std::vector<BYTE_TYPE> smallText;
    std::vector<BYTE_TYPE> mediumText;
    std::vector<BYTE_TYPE> largeText;
};

TEST_F(KalynaTest, Test128_128) {
    std::vector<WORD_TYPE> cipher;
    std::vector<BYTE_TYPE> actual_text;

    cipher = kalyna128_128.Encipher(smallText);
    actual_text = kalyna128_128.Decipher(cipher);
    ASSERT_EQ(smallText, actual_text);

    cipher = kalyna128_128.Encipher(mediumText);
    actual_text = kalyna128_128.Decipher(cipher);
    ASSERT_EQ(mediumText, actual_text);

    cipher = kalyna128_128.Encipher(largeText);
    actual_text = kalyna128_128.Decipher(cipher);
    ASSERT_EQ(largeText, actual_text);
}

TEST_F(KalynaTest, Test128_256) {
    std::vector<WORD_TYPE> cipher;
    std::vector<BYTE_TYPE> actual_text;

    cipher = kalyna128_256.Encipher(smallText);
    actual_text = kalyna128_256.Decipher(cipher);
    ASSERT_EQ(smallText, actual_text);

    cipher = kalyna128_256.Encipher(mediumText);
    actual_text = kalyna128_256.Decipher(cipher);
    ASSERT_EQ(mediumText, actual_text);

    cipher = kalyna128_256.Encipher(largeText);
    actual_text = kalyna128_256.Decipher(cipher);
    ASSERT_EQ(largeText, actual_text);
}

TEST_F(KalynaTest, Test256_256) {
    std::vector<WORD_TYPE> cipher;
    std::vector<BYTE_TYPE> actual_text;

    cipher = kalyna256_256.Encipher(smallText);
    actual_text = kalyna256_256.Decipher(cipher);
    ASSERT_EQ(smallText, actual_text);

    cipher = kalyna256_256.Encipher(mediumText);
    actual_text = kalyna256_256.Decipher(cipher);
    ASSERT_EQ(mediumText, actual_text);

    cipher = kalyna256_256.Encipher(largeText);
    actual_text = kalyna256_256.Decipher(cipher);
    ASSERT_EQ(largeText, actual_text);
}

TEST_F(KalynaTest, Test256_512) {
    std::vector<WORD_TYPE> cipher;
    std::vector<BYTE_TYPE> actual_text;

    cipher = kalyna256_512.Encipher(smallText);
    actual_text = kalyna256_512.Decipher(cipher);
    ASSERT_EQ(smallText, actual_text);

    cipher = kalyna256_512.Encipher(mediumText);
    actual_text = kalyna256_512.Decipher(cipher);
    ASSERT_EQ(mediumText, actual_text);

    cipher = kalyna256_512.Encipher(largeText);
    actual_text = kalyna256_512.Decipher(cipher);
    ASSERT_EQ(largeText, actual_text);
}

TEST_F(KalynaTest, Test512_512) {
    std::vector<WORD_TYPE> cipher;
    std::vector<BYTE_TYPE> actual_text;

    cipher = kalyna512_512.Encipher(smallText);
    actual_text = kalyna512_512.Decipher(cipher);
    ASSERT_EQ(smallText, actual_text);

    cipher = kalyna512_512.Encipher(mediumText);
    actual_text = kalyna512_512.Decipher(cipher);
    ASSERT_EQ(mediumText, actual_text);

    cipher = kalyna512_512.Encipher(largeText);
    actual_text = kalyna512_512.Decipher(cipher);
    ASSERT_EQ(largeText, actual_text);
}