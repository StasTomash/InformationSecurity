#ifndef BIGINTEGERLAB_BIGINTEGERALGORITHM_H
#define BIGINTEGERLAB_BIGINTEGERALGORITHM_H

#include "BigInteger.h"

namespace BigInt {
    static const std::vector<BigInteger> smallPrimes({BigInteger(2), // NOLINT(cert-err58-cpp)
                                                      BigInteger(3),
                                                      BigInteger(5),
                                                      BigInteger(7),
                                                      BigInteger(11),
                                                      BigInteger(13)});

    bool isPrimeMillerRabbin(const BigInteger&);
    bool smallIsPrime(const BigInteger&);
    BigInteger getPrimeOfBitLen(const int& len);
    BigInteger gcd(const BigInteger&, const BigInteger&);
    BigInteger lcm(const BigInteger&, const BigInteger&);
    BigInteger gcdExtended(const BigInteger&, const BigInteger&, BigInteger&, BigInteger&);
    BigInteger inverseInCircle(const BigInteger& arg, const BigInteger& modulo);

}

#endif //BIGINTEGERLAB_BIGINTEGERALGORITHM_H

