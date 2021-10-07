//
// Created by stast on 9/12/2021.
//

#ifndef INFORMATIONSECURITY_KALYNA_H
#define INFORMATIONSECURITY_KALYNA_H

#include <string>
#include <iostream>
#include <memory.h>
#include <cstring>
#include <utility>
#include <vector>
#include "constants.h"

#define BYTES_IN_WORD sizeof(WORD_TYPE)
#define BITS_IN_WORD (8u * BYTES_IN_WORD)
#define BITS_IN_BYTE 8u

#define K_128 (128 / BITS_IN_WORD)
#define K_256 (256 / BITS_IN_WORD)
#define K_512 (512 / BITS_IN_WORD)

#define STATE_0(s) (((s) & 0x00000000000000FFull) >> 0ull)
#define STATE_1(s) (((s) & 0x000000000000FF00ull) >> 8ull)
#define STATE_2(s) (((s) & 0x0000000000FF0000ull) >> 16ull)
#define STATE_3(s) (((s) & 0x00000000FF000000ull) >> 24ull)
#define STATE_4(s) (((s) & 0x000000FF00000000ull) >> 32ull)
#define STATE_5(s) (((s) & 0x0000FF0000000000ull) >> 40ull)
#define STATE_6(s) (((s) & 0x00FF000000000000ull) >> 48ull)
#define STATE_7(s) (((s) & 0xFF00000000000000ull) >> 56ull)

#define S_BOX_0(s_box, s) ((WORD_TYPE)((s_box)[3][STATE_0((s))]) << 0ull)
#define S_BOX_1(s_box, s) ((WORD_TYPE)((s_box)[2][STATE_1((s))]) << 8ull)
#define S_BOX_2(s_box, s) ((WORD_TYPE)((s_box)[1][STATE_2((s))]) << 16ull)
#define S_BOX_3(s_box, s) ((WORD_TYPE)((s_box)[0][STATE_3((s))]) << 24ull)
#define S_BOX_4(s_box, s) ((WORD_TYPE)((s_box)[3][STATE_4((s))]) << 32ull)
#define S_BOX_5(s_box, s) ((WORD_TYPE)((s_box)[2][STATE_5((s))]) << 40ull)
#define S_BOX_6(s_box, s) ((WORD_TYPE)((s_box)[1][STATE_6((s))]) << 48ull)
#define S_BOX_7(s_box, s) ((WORD_TYPE)((s_box)[0][STATE_7((s))]) << 56ull)

#define APPLY_S_BOX(s_box, s) S_BOX_0(s_box, s) | S_BOX_1(s_box, s) | S_BOX_2(s_box, s) | \
                              S_BOX_3(s_box, s) | S_BOX_4(s_box, s) | S_BOX_5(s_box, s) | \
                              S_BOX_6(s_box, s) | S_BOX_7(s_box, s)


#define FIELD_POLYNOMIAL 0x011Dull  //  GF(256) irreducible poly: x^8 + x^4 + x^3 + x^2 + 1


class Kalyna {
private:
    size_t blockLen{};   //  block length measured in words: 2 (128 bit), 4 (256 bit), 8 (512 bit)
    size_t keyLen{};     //  key length measured in words: 2 (128 bit), 4 (256 bit), 8 (512 bit)
    size_t roundsCnt{};  //  number of rounds: 10 (keyLen 2), 14 (keyLen 4), 18 (keyLen 8)

    bool loggingEnabled{};

    std::vector<WORD_TYPE> state;
    std::vector<std::vector<WORD_TYPE>> roundKeys;

    static bool checkKeyAndBlockLenCompatibility(
        const size_t& _keyLen,
        const size_t& _blockLen
    ) {
        if (_blockLen == K_128) {
            if (_keyLen == K_128 || _keyLen == K_256) {
                return true;
            }
        } else if (_blockLen == K_256) {
            if (_keyLen == K_256 || _keyLen == K_512) {
                return true;
            }
        } else if (_blockLen == K_512) {
            if (_keyLen == K_512) {
                return true;
            }
        }
        return false;
    }

    static std::vector<BYTE_TYPE> WordsToBytes(const std::vector<WORD_TYPE>& words) {
        std::vector<BYTE_TYPE> bytes(words.size() * BYTES_IN_WORD);
        for (size_t i = 0; i < words.size(); i++) {
            WORD_TYPE curWord = words[i];
            bytes[i * BYTES_IN_WORD]     = STATE_7(curWord);
            bytes[i * BYTES_IN_WORD + 1] = STATE_6(curWord);
            bytes[i * BYTES_IN_WORD + 2] = STATE_5(curWord);
            bytes[i * BYTES_IN_WORD + 3] = STATE_4(curWord);
            bytes[i * BYTES_IN_WORD + 4] = STATE_3(curWord);
            bytes[i * BYTES_IN_WORD + 5] = STATE_2(curWord);
            bytes[i * BYTES_IN_WORD + 6] = STATE_1(curWord);
            bytes[i * BYTES_IN_WORD + 7] = STATE_0(curWord);
        }
        return bytes;
    }

    static std::vector<WORD_TYPE> BytesToWords(const std::vector<BYTE_TYPE>& bytes) {
        std::vector<WORD_TYPE> words = std::vector<WORD_TYPE>(bytes.size() / BYTES_IN_WORD);
        for (size_t i = 0; i < words.size(); i++) {
            words[i] |= ((WORD_TYPE)bytes[i * BYTES_IN_WORD + 7]);
            words[i] |= ((WORD_TYPE)bytes[i * BYTES_IN_WORD + 6]) << 8ull;
            words[i] |= ((WORD_TYPE)bytes[i * BYTES_IN_WORD + 5]) << 16ull;
            words[i] |= ((WORD_TYPE)bytes[i * BYTES_IN_WORD + 4]) << 24ull;
            words[i] |= ((WORD_TYPE)bytes[i * BYTES_IN_WORD + 3]) << 32ull;
            words[i] |= ((WORD_TYPE)bytes[i * BYTES_IN_WORD + 2]) << 40ull;
            words[i] |= ((WORD_TYPE)bytes[i * BYTES_IN_WORD + 1]) << 48ull;
            words[i] |= ((WORD_TYPE)bytes[i * BYTES_IN_WORD]) << 56ull;
        }
        return words;
    }

    void SubBytes() {
        for (size_t i = 0; i < blockLen; i++) {
            state[i] = APPLY_S_BOX(sBoxes, state[i]);
        }
    }

    void InvSubBytes() {
        for (size_t i = 0; i < blockLen; i++) {
            state[i] = APPLY_S_BOX(sBoxesInv, state[i]);
        }
    }

    void ShiftRows() {
        std::vector<BYTE_TYPE> oldStateBytes = WordsToBytes(state);
        std::vector<BYTE_TYPE> newStateBytes = std::vector<BYTE_TYPE>(oldStateBytes.size());

        size_t shiftPeriod = BYTES_IN_WORD / blockLen;
        for (size_t row = 0, shift = 0; row < BYTES_IN_WORD; row++) {
            if (row > 0 && row % shiftPeriod == 0) {
                shift++;
            }
            for (size_t col = 0; col < blockLen; col++) {
                newStateBytes[row + ((col + shift) % blockLen) * BYTES_IN_WORD] =
                    oldStateBytes[row + col * BYTES_IN_WORD];
            }
        }
        state = BytesToWords(newStateBytes);
    }

    void InvShiftRows() {
        std::vector<BYTE_TYPE> oldStateBytes = WordsToBytes(state);
        std::vector<BYTE_TYPE> newStateBytes = std::vector<BYTE_TYPE>(oldStateBytes.size());

        size_t shiftPeriod = BYTES_IN_WORD / blockLen;
        for (size_t row = 0, shift = 0; row < BYTES_IN_WORD; row++) {
            if (row > 0 && row % shiftPeriod == 0) {
                shift++;
            }
            for (size_t col = 0; col < blockLen; col++) {
                newStateBytes[row + col * BYTES_IN_WORD] =
                    oldStateBytes[row + ((col + shift) % blockLen) * BYTES_IN_WORD];
            }
        }
        state = BytesToWords(newStateBytes);
    }

    static BYTE_TYPE MultiplyInGF(BYTE_TYPE x, BYTE_TYPE y) {
        BYTE_TYPE res = 0;
        for (size_t i = 0; i < BITS_IN_BYTE; i++) {
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

    void ApplyMixMatrix(BYTE_TYPE matrix[8][8]) {
        std::vector<BYTE_TYPE> oldStateBytes = WordsToBytes(state);
        std::vector<BYTE_TYPE> newStateBytes(oldStateBytes.size(), 0);
        for (size_t col = 0; col < blockLen; col++) {
            for (size_t row = 0; row < BYTES_IN_WORD; row++) {
                for (size_t k = 0; k < BYTES_IN_WORD; k++) {
                    newStateBytes[row + col * BYTES_IN_WORD] ^=
                        MultiplyInGF(oldStateBytes[k + col * BYTES_IN_WORD], matrix[row][k]);
                }
            }
        }
        state = BytesToWords(newStateBytes);
    }

    void MixColumns() {
        ApplyMixMatrix(mdsMatrix);
    }

    void InvMixColumns() {
        ApplyMixMatrix(mdsMatrixInv);
    }

    void AddRoundKeyInExpansion(const std::vector<WORD_TYPE>& value) {
        for (size_t i = 0; i < blockLen; i++) {
            state[i] += value[i];
        }
    }

    void AddRoundKey(size_t round) {
        for (size_t i = 0; i < blockLen; i++) {
            state[i] += roundKeys[round][i];
        }
    }

    void SubRoundKey(size_t round) {
        for (size_t i = 0; i < blockLen; i++) {
            state[i] -= roundKeys[round][i];
        }
    }

    void XorRoundKeyInExpansion(const std::vector<WORD_TYPE>& value) {
        for (size_t i = 0; i < blockLen; i++) {
            state[i] ^= value[i];
        }
    }

    void XorRoundKey(size_t round) {
        for (size_t i = 0; i < blockLen; i++) {
            state[i] ^= roundKeys[round][i];
        }
    }

    void EncipherRound() {
        SubBytes();
        ShiftRows();
        MixColumns();
    }

    void DecipherRound() {
        InvMixColumns();
        InvShiftRows();
        InvSubBytes();
    }

    void ShiftStateLeft(std::vector<WORD_TYPE>& value) const {
        for (size_t i = 0; i < blockLen; i++) {
            value[i] <<= 1ull;
        }
    }

    void RotateKeyLeft(std::vector<WORD_TYPE>& key) const {
        WORD_TYPE tmp = key[0];
        for (size_t i = 1; i < keyLen; i++) {
            key[i - 1] = key[i];
        }
        key[keyLen - 1] = tmp;
    }

    void RotateStateLeft(std::vector<WORD_TYPE>& value) const {
        size_t rotateBytes = 2 * blockLen + 3;

        auto bytes = WordsToBytes(value);

        bytes.insert(bytes.end(), bytes.begin(), bytes.begin() + rotateBytes);
        bytes.erase(bytes.begin(), bytes.begin() + rotateBytes);

        value = BytesToWords(bytes);
    }

    std::vector<WORD_TYPE> GenerateIntermediateKey(
        std::vector<WORD_TYPE> key
    ) {
        auto k0 = std::vector<WORD_TYPE>(key.begin(), key.begin() + blockLen);
        std::vector<WORD_TYPE> k1;

        std::fill(state.begin(), state.end(), 0);
        state[0] = WORD_TYPE(blockLen + keyLen + 1) << 56ull;

        if (blockLen == keyLen) {
            k1 = std::vector<WORD_TYPE>(key.begin(), key.end());
        } else {
            k1 = std::vector<WORD_TYPE>(key.begin() + blockLen, key.end());
        }

        AddRoundKeyInExpansion(k0);
        EncipherRound();
        XorRoundKeyInExpansion(k1);
        EncipherRound();
        AddRoundKeyInExpansion(k0);
        EncipherRound();

        return state;
    }

    void KeyExpandEven(
        const std::vector<WORD_TYPE>& key,
        const std::vector<WORD_TYPE>& intermediateKey
    ) {
        auto mu = std::vector<WORD_TYPE>(blockLen);
        for (size_t i = 0; i < blockLen; i++) {
            mu[i] = 0x0001000100010001ull;
        }
        auto intermediateRoundKey = std::vector<WORD_TYPE>(blockLen);
        auto originalKey = std::vector<WORD_TYPE>(key.begin(), key.end());

        size_t round = 0;
        while (true) {
            state = intermediateKey;
            AddRoundKeyInExpansion(mu);
            intermediateRoundKey = state;
            state = std::vector<WORD_TYPE>(originalKey.begin(), originalKey.begin() + blockLen);

            AddRoundKeyInExpansion(intermediateRoundKey);
            EncipherRound();
            XorRoundKeyInExpansion(intermediateRoundKey);
            EncipherRound();
            AddRoundKeyInExpansion(intermediateRoundKey);

            roundKeys[round] = state;

            if (round >= roundsCnt) {
                break;
            }

            if (keyLen != blockLen) {
                round += 2;
                ShiftStateLeft(mu);

                state = intermediateKey;
                AddRoundKeyInExpansion(mu);
                intermediateRoundKey = state;
                state = std::vector<WORD_TYPE>(originalKey.begin() + blockLen, originalKey.end());

                AddRoundKeyInExpansion(intermediateRoundKey);
                EncipherRound();
                XorRoundKeyInExpansion(intermediateRoundKey);
                EncipherRound();
                AddRoundKeyInExpansion(intermediateRoundKey);

                roundKeys[round] = state;

                if (round >= roundsCnt) {
                    break;
                }
            }

            round += 2;
            ShiftStateLeft(mu);
            RotateKeyLeft(originalKey);
        };
    }

    void KeyExpandOdd() {
        for (size_t i = 1; i <= roundsCnt; i += 2) {
            roundKeys[i] = roundKeys[i - 1];
            RotateStateLeft(roundKeys[i]);
        }
    }

    void LogState() {
        if (!loggingEnabled) {
            return;
        }
        for (size_t i = 0; i < blockLen; i++) {
            std::cerr << state[i] << " ";
        }
        std::cerr << "\n";
    }

    std::vector<WORD_TYPE> EncipherInternal(std::vector<WORD_TYPE> text) {
        state = std::move(text);
        std::cerr << "\n";
        LogState();
        AddRoundKey(0);
        LogState();
        for (size_t round = 1; round < roundsCnt; round++) {
            EncipherRound();
            LogState();
            XorRoundKey(round);
            LogState();
        }
        EncipherRound();
        LogState();
        AddRoundKey(roundsCnt);
        LogState();

        return state;
    }

    std::vector<WORD_TYPE> DecipherInternal(std::vector<WORD_TYPE> cipher) {
        state = std::move(cipher);
        std::cerr << "\n";
        LogState();
        SubRoundKey(roundsCnt);
        LogState();
        for (size_t round = roundsCnt - 1; round > 0; round--) {
            DecipherRound();
            LogState();
            XorRoundKey(round);
            LogState();
        }
        DecipherRound();
        LogState();
        SubRoundKey(0);
        LogState();

        return state;
    }
public:
    Kalyna() = default;
    bool SetKey(const size_t& _keyLen, const size_t& _blockLen, const std::vector<WORD_TYPE>& key) {
        if (!checkKeyAndBlockLenCompatibility(_keyLen, _blockLen)) {
            return false;
        }

        this->blockLen = _blockLen;
        this->keyLen = _keyLen;
        switch (_keyLen) {
            case K_128:
                this->roundsCnt = 10;
                break;
            case K_256:
                this->roundsCnt = 14;
                break;
            case K_512:
                this->roundsCnt = 18;
                break;
            default:
                break;
        }


        state = std::vector<WORD_TYPE>(blockLen);
        roundKeys = std::vector<std::vector<WORD_TYPE>>(roundsCnt + 1, std::vector<WORD_TYPE>(blockLen));

        auto intermediateKey = this->GenerateIntermediateKey(key);
        this->KeyExpandEven(key, intermediateKey);
        this->KeyExpandOdd();

        return true;
    }

    std::vector<WORD_TYPE> Encipher(std::vector<BYTE_TYPE> text) {
        size_t textLen = text.size();
        size_t textLenFit = ((textLen + blockLen * BYTES_IN_WORD - 1) / (blockLen * BYTES_IN_WORD))
            * (blockLen * BYTES_IN_WORD);
        while (text.size() < textLenFit) {
            text.push_back('\0');
        }

        std::vector<WORD_TYPE> cipher;
        for (size_t block = 0; block < textLenFit / (blockLen * BYTES_IN_WORD); block++) {
            std::vector<BYTE_TYPE> textBlock = std::vector<BYTE_TYPE>(
                text.begin() + block * blockLen * BYTES_IN_WORD,
                text.begin() + (block + 1) * blockLen * BYTES_IN_WORD
            );
            auto cipherBlock = EncipherInternal(BytesToWords(textBlock));
            cipher.insert(cipher.end(), cipherBlock.begin(), cipherBlock.end());
        }
        return cipher;
    }

    std::vector<BYTE_TYPE> Decipher(std::vector<WORD_TYPE> cipher) {
        std::vector<BYTE_TYPE> text;
        for (size_t block = 0; block < cipher.size() / blockLen; block++) {
            std::vector<WORD_TYPE> cipherBlock = std::vector<WORD_TYPE>(
                cipher.begin() + block * blockLen,
                cipher.begin() + (block + 1) * blockLen
            );
            auto textBlock = WordsToBytes(DecipherInternal(cipherBlock));
            text.insert(text.end(), textBlock.begin(), textBlock.end());
        }
        return text;
    }

    void SetLogging(bool val) {
        loggingEnabled = val;
    }

    void LogKalyna() {
        std::cout << "Block Length: " << blockLen << "\nKey Length: " << keyLen << "\nRound Num: " << roundsCnt << "\n";
        std::cout << "Round Keys: \n";
        for (int i = 0; i <= roundsCnt; i++) {
            std::cout << "Round " << i << ": ";
            for (int j = 0; j < blockLen; j++) {
                std::cout << std::hex << roundKeys[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "============\n";
    }
};

#endif //INFORMATIONSECURITY_KALYNA_H
