#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
#pragma ide diagnostic ignored "cert-msc50-cpp"
#include <fstream>
#include "AES/AES.h"
#include "Kalyna/kalyna.h"
#include "RC4/rc4.h"
#include "Salsa20/salsa20.h"
#include <random>
#include <chrono>
#include <cassert>

#define MAX_LENGTH 100 * 1024 * 1024
#define MEASURE_TIME(func, ans) { \
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); \
    func; \
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); \
    ans = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); \
}

std::vector<unsigned int> GenerateWordVector(size_t len) {
    std::vector<unsigned int> res(len);
    for (int i = 0; i < len; i++) {
        res[i] = (unsigned)rand() | ((unsigned)rand() << 16u);
    }
    return res;
}

unsigned char* GenerateCharArray(size_t len) {
    auto* arr = (unsigned char*)malloc(len + 1);
    for (int i = 0; i < len; i++) {
        arr[i] = rand() % 255 + 1;
    }
    arr[len] = '\0';
    return arr;
}

void BenchmarkAESECB(unsigned char* data, size_t arrayLen) {
    AES aes;
    auto key = GenerateCharArray(16);
    aes.SetKey(4, key);

    std::string dataString((char*)data), cipher, result;
    int t;
    MEASURE_TIME(cipher = aes.EncryptECB(dataString), t);
    printf("AES ECB encryption of %d bytes of data took %dms.\n", arrayLen, t);
    MEASURE_TIME(result = aes.DecryptECB(cipher), t);
    printf("AES ECB decryption of %d bytes of data took %dms.\n", arrayLen, t);

    assert(result == dataString);
}

void BenchmarkAESOFB(unsigned char* data, size_t arrayLen) {
    AES aes;
    auto key = GenerateCharArray(16), saltBytes = GenerateCharArray(16);
    aes.SetKey(4, key);
    std::string salt((char*)saltBytes);

    std::string dataString((char*)data), cipher, result;
    int t;
    MEASURE_TIME(cipher = aes.EncryptOFB(dataString, salt), t);
    printf("AES OFB encryption of %d bytes of data took %dms.\n", arrayLen, t);
    MEASURE_TIME(result = aes.DecryptOFB(cipher, salt), t);
    printf("AES OFB decryption of %d bytes of data took %dms.\n", arrayLen, t);

    assert(result == dataString);
}

void BenchmarkAESCBC(unsigned char* data, size_t arrayLen) {
    AES aes;
    auto key = GenerateCharArray(16), saltBytes = GenerateCharArray(16);
    aes.SetKey(4, key);
    std::string salt((char*)saltBytes);

    std::string dataString((char*)data), cipher, result;
    int t;
    MEASURE_TIME(cipher = aes.EncryptCBC(dataString, salt), t);
    printf("AES CBC encryption of %d bytes of data took %dms.\n", arrayLen, t);
    MEASURE_TIME(result = aes.DecryptCBC(cipher, salt), t);
    printf("AES CBC decryption of %d bytes of data took %dms.\n", arrayLen, t);

    assert(result == dataString);
}

void BenchmarkAESCFB(unsigned char* data, size_t arrayLen) {
    AES aes;
    auto key = GenerateCharArray(16), saltBytes = GenerateCharArray(16);
    aes.SetKey(4, key);
    std::string salt((char*)saltBytes);

    std::string dataString((char*)data), cipher, result;
    int t;
    MEASURE_TIME(cipher = aes.EncryptCFB(dataString, salt), t);
    printf("AES CFB encryption of %d bytes of data took %dms.\n", arrayLen, t);
    MEASURE_TIME(result = aes.DecryptCFB(cipher, salt), t);
    printf("AES CFB decryption of %d bytes of data took %dms.\n", arrayLen, t);

    assert(result == dataString);
}

void BenchmarkAESCTR(unsigned char* data, size_t arrayLen) {
    AES aes;
    auto key = GenerateCharArray(16), saltBytes = GenerateCharArray(8);
    aes.SetKey(4, key);
    std::string salt((char*)saltBytes);

    std::string dataString((char*)data), cipher, result;
    int t;
    MEASURE_TIME(cipher = aes.EncryptCTR(dataString, salt), t);
    printf("AES CTR encryption of %d bytes of data took %dms.\n", arrayLen, t);
    MEASURE_TIME(result = aes.DecryptCTR(cipher, salt), t);
    printf("AES CTR decryption of %d bytes of data took %dms.\n", arrayLen, t);

    assert(result == dataString);
}

void BenchmarkKalyna(const unsigned char* data, size_t arrayLen) {
    std::vector<BYTE_TYPE> dataVector(arrayLen), result;
    for (int i = 0; i < dataVector.size(); i++) {
        dataVector[i] = data[i];
    }

    auto key128 = std::vector<WORD_TYPE>( { 0x0001020304050607ull, 0x08090A0B0C0D0E0Full });
    auto key512 = std::vector<WORD_TYPE>({ 0x0001020304050607ull, 0x08090A0B0C0D0E0Full,
                                              0x0001020304050607ull, 0x08090A0B0C0D0E0Full,
                                              0x0001020304050607ull, 0x08090A0B0C0D0E0Full,
                                              0x0001020304050607ull, 0x08090A0B0C0D0E0Full, });

    Kalyna kalyna128_128, kalyna512_512;

    kalyna128_128.Configure(K_128, K_128);
    kalyna128_128.SetKey(key128);
    kalyna512_512.Configure(K_512, K_512);
    kalyna512_512.SetKey(key512);

    std::vector<WORD_TYPE> cipher;
    int t;
    MEASURE_TIME(cipher = kalyna128_128.Encipher(dataVector), t);
    printf("Kalyna (128 block, 128 key) encryption of %d bytes of data took %dms.\n", arrayLen, t);
    MEASURE_TIME(result = kalyna128_128.Decipher(cipher), t);
    printf("Kalyna (128 block, 128 key) decryption of %d bytes of data took %dms.\n", arrayLen, t);
    assert(result == dataVector);

    MEASURE_TIME(cipher = kalyna512_512.Encipher(dataVector), t);
    printf("Kalyna (512 block, 512 key) encryption of %d bytes of data took %dms.\n", arrayLen, t);
    MEASURE_TIME(result = kalyna512_512.Decipher(cipher), t);
    printf("Kalyna (512 block, 512 key) decryption of %d bytes of data took %dms.\n", arrayLen, t);
    assert(result == dataVector);
}

void BenchmarkRC4(const unsigned char* data, size_t arrayLen) {
    RC4 rc4;
    auto keyBytes = GenerateCharArray(256);
    std::vector<unsigned char> key(256), dataVector(arrayLen), cipher, result;
    for (int i = 0; i < 256; i++) {
        key[i] = keyBytes[i];
    }
    for (int i = 0; i < arrayLen; i++) {
        dataVector[i] = data[i];
    }

    rc4.Init(key);

    int t;
    MEASURE_TIME(cipher = rc4.Process(dataVector), t);
    printf("RC4 encryption of %d bytes of data took %dms.\n", arrayLen, t);
    MEASURE_TIME(result = rc4.Process(cipher), t);
    printf("RC4 decryption of %d bytes of data took %dms.\n", arrayLen, t);

    assert(result == dataVector);
}

void BenchmarkSalsa20(const unsigned char* data, size_t arrayLen) {
    Salsa20 salsa20;

    std::vector<unsigned char> cipher, dataVector(arrayLen), result;
    std::vector<unsigned int> key;

    for (int i = 0; i < arrayLen; i++) {
        dataVector[i] = data[i];
    }

    key = GenerateWordVector(8);
    salsa20.SetKey(key);

    int t;
    MEASURE_TIME(cipher = salsa20.Process(dataVector), t);
    printf("RC4 encryption of %d bytes of data took %dms.\n", arrayLen, t);
    MEASURE_TIME(result = salsa20.Process(cipher), t);
    printf("RC4 decryption of %d bytes of data took %dms.\n", arrayLen, t);

    assert(result == dataVector);
}

int main(int argc, char** argv) {
    size_t arrayLen = MAX_LENGTH;
    if (argc > 1) {
        arrayLen = std::min(arrayLen, (size_t)atoi(argv[1]));
    }
    auto arr = GenerateCharArray(arrayLen);
    BenchmarkAESECB(arr, arrayLen);
    BenchmarkKalyna(arr, arrayLen);
    BenchmarkAESOFB(arr, arrayLen);
    BenchmarkAESCBC(arr, arrayLen);
    BenchmarkAESCTR(arr, arrayLen);
    BenchmarkAESCFB(arr, arrayLen);
    BenchmarkRC4(arr, arrayLen);
    BenchmarkSalsa20(arr, arrayLen);
}