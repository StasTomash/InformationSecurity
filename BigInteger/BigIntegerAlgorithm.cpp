#include "BigIntegerAlgorithm.h"
#include <map>
#include <algorithm>
#include <set>

bool BigInt::isPrimeMillerRabbin(const BigInt::BigInteger& arg) {
    if (arg < BigInteger(1000000)) {
        return smallIsPrime(arg);
    }
    BigInteger curPower(1), curExp(0);
    while ((arg - BigInteger(1)) % (curPower * BigInteger(2)) == BigInteger(0)) {
        curPower *= BigInteger(2);
        curExp++;
    }
    BigInteger rem((arg - BigInteger(1)) / curPower);

    for (const auto& x : smallPrimes) {
        if (x.pow(rem, arg) == BigInteger(1)) {
            continue;
        }
        bool isEvidence = false;
        BigInteger curTrial = x.pow(rem, arg);
        for (BigInteger i(0); i < curExp; i++) {
            if (curTrial == arg - BigInteger(1)) {
                isEvidence = true;
                break;
            }
            curTrial = (curTrial * curTrial) % arg;
        }
        if (!isEvidence) {
            return false;
        }
    }
    return true;
}

bool BigInt::smallIsPrime(const BigInt::BigInteger& arg) {
    int cp = arg.toInt();
    if (cp == 1) {
        return false;
    }
    for (int i=2; i*i <= cp; i++) {
        if (cp % i == 0) {
            return false;
        }
    }
    return true;
}

BigInt::BigInteger BigInt::getPrimeOfBitLen(const int& len) {
    while (true) {
        BigInt::BigInteger candidate = BigInt::BigInteger::getRandOfBitLen(len);
        if (BigInt::isPrimeMillerRabbin(candidate)) {
            return candidate;
        }
    }
}

BigInt::BigInteger BigInt::gcd(const BigInt::BigInteger& a, const BigInt::BigInteger& b) {
    if (a == BigInteger(0) || b == BigInteger(0)) {
        return a + b;
    }
    return gcd(b, a % b);
}

BigInt::BigInteger BigInt::lcm(const BigInt::BigInteger& a, const BigInt::BigInteger& b) {
    return (a * b) / gcd(a, b);
}

BigInt::BigInteger BigInt::gcdExtended(const BigInt::BigInteger &a, const BigInt::BigInteger &b, BigInt::BigInteger &x,
                                       BigInt::BigInteger &y) {
    if (a == BigInteger(0)) {
        x = BigInteger(0);
        y = BigInteger(1);
        return b;
    }
    BigInteger x1, y1, d;
    d = gcdExtended(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

BigInt::BigInteger BigInt::inverseInCircle(const BigInt::BigInteger &arg, const BigInteger& modulo) {
    BigInteger x, y, g;
    g = BigInt::gcdExtended(arg, modulo, x, y);
    if (g != BigInteger(1)) {
        throw;
    }
    return (x + modulo) % modulo;
}
