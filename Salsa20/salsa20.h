#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#ifndef INFORMATIONSECURITY_SALSA20_H
#define INFORMATIONSECURITY_SALSA20_H

#include <vector>
#include <random>

#define ROUNDS 20

class Salsa20 {
private:
    std::vector<unsigned int> state;
    void QuarterRound(size_t a, size_t b, size_t c, size_t d) {
        state[b] ^= (state[a] + state[d]) << 7u;
        state[c] ^= (state[b] + state[a]) << 9u;
        state[d] ^= (state[c] + state[b]) << 13u;
        state[a] ^= (state[d] + state[c]) << 18u;
    }
    void OddRound() {
        QuarterRound(0, 4, 8, 12);
        QuarterRound(1, 5, 9, 13);
        QuarterRound(2, 6, 10, 14);
        QuarterRound(3, 7, 11, 15);
    }
    void EvenRound() {
        QuarterRound(0, 1, 2, 3);
        QuarterRound(4, 5, 6, 7);
        QuarterRound(8, 9, 10, 11);
        QuarterRound(12, 13, 14, 15);
    }
    void DoubleRound() {
        OddRound();
        EvenRound();
    }
    std::vector<unsigned char> GetCipherKey() {
        std::vector<unsigned char> res;
        for (int i = 0; i < 16; i++) {
            res.push_back((state[i] >> 24u) & 0x11u);
            res.push_back((state[i] >> 16u) & 0x11u);
            res.push_back((state[i] >> 8u) & 0x11u);
            res.push_back((state[i] >> 0u) & 0x11u);
        }
        return res;
    }
public:
    void SetKey(const std::vector<unsigned int>& key) {
        state = { 0x65787061u, key[0], key[1], key[2],
                  key[3], 0x6E642033u, static_cast<unsigned int>(rand()), static_cast<unsigned int>(rand()),
                  0, 0, 0x322D6279u, key[4],
                  key[5], key[6], key[7], 0x7565206bu };
        for (int i = 0; i < ROUNDS; i += 2) {
            DoubleRound();
        }
        for (int i = 0; i < 16; i++) {
            state[i] = key[i] + state[i];
        }
    }
    std::vector<unsigned char> Process(const std::vector<unsigned char>& data) {
        std::vector<unsigned char> res(data.size());
        auto cipherKey = GetCipherKey();
        for (int i = 0; i < data.size(); i++) {
            res[i] = data[i] ^ cipherKey[i % cipherKey.size()];
        }
        return res;
    }
};

#endif //INFORMATIONSECURITY_SALSA20_H
