#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err58-cpp"
//
// Created by stast on 11/14/2021.
//

#include "rsa.h"
#include <gtest/gtest.h>

TEST(RSATest, RSATest) {
    auto keyPair = RSA::RSAGenKeyPair(64);
    std::string msg = "test msg!";
    auto cipher = RSA::RSAEncrypt(msg, keyPair.publicKey);
    ASSERT_EQ(msg, RSA::RSADecrypt(cipher, keyPair.privateKey));
}

TEST(RSATest, RSATestSignature) {
    auto keyPair = RSA::RSAGenKeyPair(64);
    auto otherKeyPair = RSA::RSAGenKeyPair(64);
    std::string msg = "test msg";
    auto cipher = RSA::RSAEncrypt(msg, keyPair.publicKey);
    auto signature = RSA::RSASign(msg, keyPair.privateKey);
    ASSERT_EQ(msg, RSA::RSADecrypt(cipher, keyPair.privateKey));
    ASSERT_NE(msg, RSA::RSADecrypt(cipher, otherKeyPair.privateKey));
    ASSERT_TRUE(RSA::RSAVerifySignature(msg, keyPair.publicKey, signature));
    auto badSignature = RSA::RSASign(msg, otherKeyPair.privateKey);
    ASSERT_FALSE(RSA::RSAVerifySignature(msg, keyPair.publicKey, badSignature));
    ASSERT_FALSE(RSA::RSAVerifySignature(msg, otherKeyPair.publicKey, signature));
    ASSERT_TRUE(RSA::RSAVerifySignature(msg, otherKeyPair.publicKey, badSignature));
}