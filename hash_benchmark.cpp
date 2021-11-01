#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#include "Kupyna256/kupyna256.h"
#include "SHA256/sha256.h"
#include <random>
#include <vector>
#include <chrono>
#include <iostream>

#define POW_ROUNDS 100

#define MEASURE_TIME(func, ans) { \
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now(); \
    func; \
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now(); \
    ans = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count(); \
}

bool IsProofOfWork(std::vector<unsigned int> hash, size_t targetPrefix) {
    for (int i = 0; i < targetPrefix; i += 32) {
        if (i + 32 <= targetPrefix) {
            if (hash[i] != 0) {
                return false;
            }
        } else {
            size_t rem = targetPrefix - i;
            unsigned int target = 0;
            for (size_t p = 31; p > 31 - rem; p--) {
                target |= (1u << p);
            }
            if (hash[i] & target) {
                return false;
            }
        }
    }
    return true;
}

template <class T> std::vector<unsigned int> ProofOfWork(T hashClass, size_t targetPrefix, int& iterations) {
    std::vector<unsigned char> vec(64);
    while (true) {
        iterations++;
        for (int i = 0; i < 64; i++) {
            vec[i] = rand() % 256;
        }
        std::vector<unsigned int> hash = hashClass.GetHash(vec);
        if (IsProofOfWork(hash, targetPrefix)) {
            return hash;
        }
    }
}

int main() {
    Kupyna256 kupyna256{};
    SHA256 sha256{};

    int kupynaTime = 0, kupynaIterations = 0;
    int shaTime = 0, shaIterations = 0;

    for (int i = 0; i < POW_ROUNDS; i++) {
        int t;
//        MEASURE_TIME(ProofOfWork(kupyna256, 12, kupynaIterations), t);
//        std::cout << "Kupyna " << i << "\n";
//        kupynaTime += t;
        MEASURE_TIME(ProofOfWork(sha256, 15, shaIterations), t);
        std::cout << "SHA " << i << "\n";
        shaTime += t;
    }

//    printf("Kupyna-256 proof-of-work time (12 bits) is %d ms and %d iterations in average\n", kupynaTime / POW_ROUNDS, kupynaIterations / POW_ROUNDS);
    printf("SHA-256 proof-of-work time (15 bits) is %d ms and %d iterations in average\n", shaTime / POW_ROUNDS, shaIterations / POW_ROUNDS);
}