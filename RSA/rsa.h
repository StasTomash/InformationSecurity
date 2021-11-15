//
// Created by stast on 11/14/2021.
//

#ifndef INFORMATIONSECURITY_RSA_H
#define INFORMATIONSECURITY_RSA_H

#include "../BigInteger/BigIntegerAlgorithm.h"
#include <random>
#include <algorithm>



struct RSAPrivateKey {
    BigInt::BigInteger p, q;    // primes
    BigInt::BigInteger n;       // p * q
    BigInt::BigInteger d;       // e ^ -1 (lcm((p - 1),(q - 1)))
    BigInt::BigInteger dP, dQ;  // d (mod (p - 1)) and d (mod (q - 1))
    BigInt::BigInteger qInv;    // q ^ -1 (mod p)

    RSAPrivateKey() = default;
    RSAPrivateKey(const RSAPrivateKey& other) = default;
    RSAPrivateKey(BigInt::BigInteger _p, BigInt::BigInteger _q, BigInt::BigInteger _e) : p(_p), q(_q) {
        n = p * q;
        d = BigInt::inverseInCircle(_e, lcm(p - BigInt::BigInteger(1), q - BigInt::BigInteger(1)));
        dP = d % (p - BigInt::BigInteger(1));
        dQ = d % (q - BigInt::BigInteger(1));
        qInv = BigInt::inverseInCircle(q, p);
    }
};

struct RSAPublicKey {
    BigInt::BigInteger n;
    BigInt::BigInteger e;

    RSAPublicKey() = default;
    RSAPublicKey(const RSAPublicKey& other) = default;
    RSAPublicKey(BigInt::BigInteger _p, BigInt::BigInteger _q, BigInt::BigInteger _e) : n(_p * _q), e(_e) {}

    bool operator == (const RSAPublicKey& other) const {
        return (n == other.n && e == other.e);
    }
};

struct RSAKeyPair {
    RSAPrivateKey privateKey;
    RSAPublicKey publicKey;
    RSAKeyPair() = default;
    RSAKeyPair(const RSAKeyPair& other) = default;
};

class RSA {
private:
    static const int DIVERSITY = 8;
    static const int DEFAULT_E = 65537;

    static BigInt::BigInteger RSAEncryptInternal(size_t msg, const RSAPublicKey& key) {
        return BigInt::BigInteger(std::to_string(msg)).pow(key.e, key.n);
    }
    static std::string RSADecryptInternal(const BigInt::BigInteger& msg, const RSAPrivateKey& key) {
        BigInt::BigInteger m1 = msg.pow(key.dP, key.p);
        BigInt::BigInteger m2 = msg.pow(key.dQ, key.q);
        BigInt::BigInteger h = (key.qInv * (m1 - m2)) % key.p;
        size_t decr = ((m2 + h * key.q) % (key.n)).toInt();
        std::string ans;
        while (decr) {
            ans += char(decr % 256);
            decr /= 256;
        }
        std::reverse(ans.begin(), ans.end());
        return ans;
    }
    static std::string RSADecryptInternalStupid(const BigInt::BigInteger& msg, const RSAPrivateKey& key) {
        size_t decr = msg.pow(key.d, key.n).toInt();
        std::string ans;
        while (decr) {
            ans += char(decr % 256);
            decr /= 256;
        }
        std::reverse(ans.begin(), ans.end());
        return ans;
    }
    static std::vector<size_t> SplitMsg(const std::string& msg) {
        std::vector<size_t> ans;
        size_t cur = 0;
        for (int i = 0; i < msg.size(); i++) {
            if (i && (i % 4 == 0)) {
                ans.push_back(cur);
                cur = 0;
            }
            cur *= 256;
            cur += msg[i];
        }
        if (cur) {
            ans.push_back(cur);
        }
        return ans;
    }
public:
    static const size_t DEFAULT_KEY_LEN = 80;
    static RSAKeyPair RSAGenKeyPair(int keyLen = DEFAULT_KEY_LEN, int _e = DEFAULT_E) {
        std::uniform_int_distribution<int> distribution(-DIVERSITY, DIVERSITY);
        static std::random_device randomDevice;
        int d1 = distribution(randomDevice);
        int d2 = DIVERSITY - d1;
        BigInt::BigInteger p = BigInt::getPrimeOfBitLen(keyLen / 2 + d1);
        BigInt::BigInteger q = BigInt::getPrimeOfBitLen(keyLen / 2 + d2);
        BigInt::BigInteger e = BigInt::BigInteger(_e);

        return RSAKeyPair{RSAPrivateKey(p, q, e), RSAPublicKey(p, q, e)};
    }
    static std::vector<BigInt::BigInteger> RSAEncrypt(const std::string& msg, const RSAPublicKey& key) {
        size_t msgLen = msg.length();
        std::vector<size_t> batches = SplitMsg(msg);
        std::vector<BigInt::BigInteger> ans;
        ans.reserve(batches.size());
        for (auto batch : batches) {
            ans.push_back(RSAEncryptInternal(batch, key));
        }
        return ans;
    }
    static std::string RSADecrypt(const std::vector<BigInt::BigInteger>& msg, const RSAPrivateKey& key) {
        size_t msgLen = msg.size();
        std::string ans;
        for (int i = 0; i < msgLen; i++) {
            ans += RSADecryptInternal(msg[i], key);
        }
        return ans;
    }
    static std::string RSADecryptStupid(const std::vector<BigInt::BigInteger>& msg, const RSAPrivateKey& key) {
        size_t msgLen = msg.size();
        std::string ans;
        for (int i = 0; i < msgLen; i++) {
            ans += RSADecryptInternalStupid(msg[i], key);
        }
        return ans;
    }
    static BigInt::BigInteger ProduceHash(const std::string& msg) {
        int h = ((int)std::hash<std::string>{}(msg));
        h = abs(h);
        return BigInt::BigInteger(h);
    }
    static BigInt::BigInteger RSASign(const std::string& msg, const RSAPrivateKey& key) {
        BigInt::BigInteger h = ProduceHash(msg);
        return h.pow(key.d, key.n);
    }
    static bool RSAVerifySignature(const std::string& msg, const RSAPublicKey& key, const BigInt::BigInteger& signature) {
        BigInt::BigInteger h = ProduceHash(msg);
        BigInt::BigInteger expected = signature.pow(key.e, key.n);
        return (h == expected);
    }
};

#endif //INFORMATIONSECURITY_RSA_H
