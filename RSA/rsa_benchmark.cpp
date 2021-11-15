#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
//
// Created by stast on 11/14/2021.
//

#include "rsa.h"
#include <chrono>
#include <cassert>

#define MEASURE_TIME(func, ans) { \
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); \
    func; \
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); \
    ans = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); \
}

int main() {
    std::string message;
    for (int i = 0; i < 256; i++) {
        message += char(rand() % 60 + 60);
    }
    std::vector<int> keySizes( { 64, 96, 128 } );
    for (auto keyLen : keySizes) {
        RSAKeyPair keyPair;
        size_t time;
        MEASURE_TIME(keyPair = RSA::RSAGenKeyPair(keyLen), time);
        printf("Key generation for %d bits key len is %d ms\n", keyLen, time);
        std::vector<BigInt::BigInteger> cipher;
        MEASURE_TIME(cipher = RSA::RSAEncrypt(message, keyPair.publicKey), time);
        printf("Encryption for %d bits key len is %d ms\n", keyLen, time);
        std::string res;
        MEASURE_TIME(res = RSA::RSADecrypt(cipher, keyPair.privateKey), time);
        printf("Decryption for %d bits key len is %d ms\n", keyLen, time);
        assert(res == message);
        MEASURE_TIME(res = RSA::RSADecryptStupid(cipher, keyPair.privateKey), time);
        printf("Decryption for %d bits key len is %d ms (without crt)\n", keyLen, time);
        assert(res == message);

    }
}