//
// Created by stast on 10/31/2021.
//

#ifndef INFORMATIONSECURITY_KUPYNA256_H
#define INFORMATIONSECURITY_KUPYNA256_H

#include "constants.h"

#define FIELD_POLYNOMIAL 0x011Dull
#define ROUNDS 10

class Kupyna256 {
private:
    unsigned char state[8][8];
    std::vector<unsigned char> initializerHash = { 0x01u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                  0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u };
    template<typename T> std::vector<unsigned char> ConvertToChar(std::vector<T> data) {
        int typeLen = sizeof(T);
        std::vector<unsigned char> charData;
        for (auto item : data) {
            auto* charItem = reinterpret_cast<unsigned char*>(&item);
            for (int i = 0; i < typeLen; i++) {
                charData.push_back(charItem[typeLen - i - 1]);
            }
        }
        return charData;
    }
    static void PadCharData(std::vector<unsigned char>& data) {
        auto nextChar = (unsigned char)(1u << 7u);
        size_t bitLen = data.size() * 8;
        size_t zeroBits = 512 - (bitLen + 97) % 512;
        assert(zeroBits >= 7);
        zeroBits -= 7;
        data.push_back(nextChar);
        while (zeroBits > 0) {
            data.push_back(0);
            zeroBits -= 8;
        }
        unsigned long long bitLen64 = bitLen;
        auto* bitLenChar = reinterpret_cast<unsigned char*>(&bitLen64);
        for (int i = 0; i < 4; i++) {
            data.push_back(0);
        }
        for (int i = 0; i < 8; i++) {
            data.push_back(bitLenChar[7 - i]);
        }
    }
    void AddRoundKeyMod2(size_t round) {
        for (size_t i = 0; i < 8; i++) {
            state[0][i] ^= (i << 4u) ^ round;
        }
    }
    unsigned long long ColumnToUInt64(size_t columnNum) {
        return (
            (unsigned long long)state[columnNum][0] << 56ull |
            (unsigned long long)state[columnNum][1] << 48ull |
            (unsigned long long)state[columnNum][2] << 40ull |
            (unsigned long long)state[columnNum][3] << 32ull |
            (unsigned long long)state[columnNum][4] << 24ull |
            (unsigned long long)state[columnNum][5] << 16ull |
            (unsigned long long)state[columnNum][6] << 8ull |
            (unsigned long long)state[columnNum][7] << 0ul
        );
    }
    void UInt64ToColumn(size_t columnNum, unsigned long long x) {
        state[columnNum][0] = x >> 56ull;
        state[columnNum][1] = x >> 48ull;
        state[columnNum][2] = x >> 40ull;
        state[columnNum][3] = x >> 32ull;
        state[columnNum][4] = x >> 24ull;
        state[columnNum][5] = x >> 16ull;
        state[columnNum][6] = x >> 8ull;
        state[columnNum][7] = x >> 0ull;
    }
    void AddRoundKeyMod64(size_t round) {
        for (int i = 0; i < 8; i++) {
            unsigned long long x = ColumnToUInt64(i);
            x += 0xF3F0F0F0F0F0F000ull ^ ((7ull - i) << 4ull) ^ round;
            UInt64ToColumn(i, x);
        }
    }
    void SubBytes() {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                state[i][j] = sBoxes[i % 4][state[i][j]];
            }
        }
    }
    void ShiftBytes() {
        int shift = 0;
        for (auto &row : state) {
            unsigned char newRow[8];
            for (int j = 0; j < 8; j++) {
                newRow[(j + shift) % 8] = row[j];
            }
            for (int j = 0; j < 8; j++) {
                row[j] = newRow[j];
            }
            shift++;
        }
    }
    static unsigned char MultiplyInGF(unsigned char x, unsigned char y) {
        unsigned char res = 0;
        for (size_t i = 0; i < 8; i++) {
            if (y & 0x1u) {
                res ^= x;
            }
            auto hi_bit = x & 0x80u;
            x <<= 1u;
            if (hi_bit) {
                x ^= FIELD_POLYNOMIAL;
            }
            y >>= 1u;
        }
        return res;
    }
    void MixColumns() {
        std::vector<std::vector<unsigned char> > newState(8, std::vector<unsigned char>(8, 0));
        for (size_t col = 0; col < 8; col++) {
            for (size_t row = 0; row < 8; row++) {
                for (size_t k = 0; k < 8; k++) {
                    newState[row][col] ^= MultiplyInGF(mdsMatrix[row][k], state[k][col]);
                }
            }
        }
        for (size_t i = 0; i < 8; i++) {
            for (size_t j = 0; j < 8; j++) {
                state[i][j] = newState[i][j];
            }
        }
    }
    std::vector<unsigned char> StateToSeq() {
        std::vector<unsigned char> res;
        for (int i = 0; i < 8; i++) {
            for (auto &row : state) {
                res.push_back(row[i]);
            }
        }
        return res;
    }
    std::vector<unsigned char> TransformationT1(std::vector<unsigned char> seq) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                state[i][j] = seq[i + j * 8];
            }
        }
        for (int i = 0; i < ROUNDS; i++) {
            AddRoundKeyMod2(i);
            SubBytes();
            ShiftBytes();
            MixColumns();
        }
        return StateToSeq();
    }
    std::vector<unsigned char> TransformationT2(std::vector<unsigned char> seq) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                state[i][j] = seq[i + j * 8];
            }
        }
        for (int i = 0; i < ROUNDS; i++) {
            AddRoundKeyMod64(i);
            SubBytes();
            ShiftBytes();
            MixColumns();
        }
        return StateToSeq();
    }
    static std::vector<unsigned char> SeqXor(std::vector<unsigned char> x, std::vector<unsigned char> y) {
        std::vector<unsigned char> seq;
        seq.reserve(x.size());
        for (int i = 0; i < x.size(); i++) {
            seq.push_back(x[i] ^ y[i]);
        }
        return seq;
    }
    std::vector<unsigned char> ProcessBlock(const std::vector<unsigned char>& prevHash, const std::vector<unsigned char>& block) {
        return SeqXor(TransformationT1(SeqXor(prevHash, block)), SeqXor(TransformationT2(block), prevHash));
    }
    std::vector<unsigned int> FormHash(const std::vector<unsigned char>& hash) {
        std::vector<unsigned char> finalHash = SeqXor(TransformationT1(hash), hash);
        std::vector<unsigned int> ans;
        for (int i = 0; i < 32; i += 4) {
            ans.push_back(((unsigned int)(finalHash[i]) << 24u) |
                          ((unsigned int)(finalHash[i + 1]) << 16u) |
                          ((unsigned int)(finalHash[i + 2]) << 8u) |
                          ((unsigned int)(finalHash[i + 3]) << 0u));
        }
        return ans;
    }

public:
    template<typename T> std::vector<unsigned int> GetHash(std::vector<T> data) {
        auto convertedData = ConvertToChar(data);
        PadCharData(convertedData);
        std::vector<unsigned char> hash = initializerHash;
        for (int blockNum = 0; blockNum < convertedData.size() / 64; blockNum++) {
            std::vector<unsigned char> block(convertedData.begin() + blockNum * 64, convertedData.begin() + (blockNum + 1) * 64);
            hash = ProcessBlock(hash, block);
        }
        return FormHash(hash);
    }
};

#endif //INFORMATIONSECURITY_KUPYNA256_H
