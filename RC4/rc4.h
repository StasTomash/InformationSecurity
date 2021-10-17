#ifndef INFORMATIONSECURITY_RC4_H
#define INFORMATIONSECURITY_RC4_H

#include <vector>

class RC4 {
private:
    std::vector<unsigned char> state;
    unsigned char GetNextRandom(size_t& i, size_t& j) {
        i = (i + 1) % 256;
        j = (j + state[i]) % 256;
        std::swap(state[i], state[j]);
        return state[(state[i] + state[j]) % 256];
    }
public:
    void Init(const std::vector<unsigned char>& key) {
        state.clear();
        for (int i = 0; i < 256; i++) {
            state.push_back(i);
        }
        for (int i = 0, j = 0; i < 256; i++) {
            j = (j + key[i % key.size()] + state[i]) % 256;
            std::swap(state[i], state[j]);
        }
    }

    std::vector<unsigned char> Process(const std::vector<unsigned char>& data) {
        auto originalState = state;
        std::vector<unsigned char> res(data.size());
        for (size_t i = 0, j = 0, pos = 0; pos < data.size(); pos++) {
            unsigned char randNum = GetNextRandom(i, j);
            res[pos] = data[pos] ^ randNum;
        }
        state = originalState;
        return res;
    }
};

#endif //INFORMATIONSECURITY_RC4_H
