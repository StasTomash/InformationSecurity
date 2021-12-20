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
    static const BigInteger DEFAULT_CURVE_BASE = BigInteger("340282366762482138434845932244680310783"); // NOLINT(cert-err58-cpp)
    static const BigInteger DEFAULT_CURVE_PARAM_A = BigInteger("1111214402954562684767210630172113326"); // NOLINT(cert-err58-cpp)
    static const BigInteger DEFAULT_CURVE_PARAM_B = BigInteger("492923134286678605454115478242553226"); // NOLINT(cert-err58-cpp)
    static const BigInteger DEFAULT_X = BigInteger("640815588627398489149279318676728769"); // NOLINT(cert-err58-cpp)
    static const BigInteger DEFAULT_Y = BigInteger("206202497085686037520439283583796043"); // NOLINT(cert-err58-cpp)
    static const BigInteger DEFAULT_ORDER = BigInteger("4451685225093714776491891542548933");

    bool isPrimeMillerRabbin(const BigInteger&);
    bool smallIsPrime(const BigInteger&);
    BigInteger getPrimeOfBitLen(const int& len);
    BigInteger gcd(const BigInteger&, const BigInteger&);
    BigInteger lcm(const BigInteger&, const BigInteger&);
    BigInteger gcdExtended(const BigInteger&, const BigInteger&, BigInteger&, BigInteger&);
    BigInteger InverseInCircle(const BigInteger& arg, const BigInteger& modulo);

    struct KeyPair;
    class EllipticCurve {
        BigInteger a;
        BigInteger b;
        BigInteger p;
        public:
        EllipticCurve() : a(DEFAULT_CURVE_PARAM_A), b(DEFAULT_CURVE_PARAM_B), p(DEFAULT_CURVE_BASE) {}
        EllipticCurve(const BigInteger& _a, const BigInteger& _b, const BigInteger& _p) : a(_a), b(_b), p(_p) {}
        BigInteger GetBase() const {
            return p;
        }
        BigInteger GetGeneratorOrder() const {
            return DEFAULT_ORDER;
        }
        class Point {
            BigInteger x;
            BigInteger y;
            EllipticCurve* parent;
            public:
            Point(const BigInteger& _x, const BigInteger& _y, EllipticCurve* _parent) :
                x(_x), y(_y), parent(_parent) {}
            Point(const Point&) = default;
            BigInt::BigInteger GetX() const {
                return x;
            }
            BigInt::BigInteger GetY() const {
                return y;
            }
            explicit Point(EllipticCurve* _parent) :
                x(DEFAULT_X), y(DEFAULT_Y), parent(_parent) {}
            Point operator+(const Point& other) const;
            Point operator*(const BigInteger& other) const;
            bool operator==(const Point& other) const;
            Point inverse() const;
            friend std::ostream& operator<<(std::ostream& os, const EllipticCurve::Point& pt);
            EllipticCurve* getParent() const {
                return parent;
            }
        };
        Point createPoint(const BigInteger& x, const BigInteger& y);
        Point createPoint();
        friend std::ostream& operator<<(std::ostream& os, const EllipticCurve& pt);
    };

    struct  KeyPair {
        EllipticCurve::Point publicKey;
        BigInteger privateKey;
    };
    std::ostream& operator<<(std::ostream& os, const EllipticCurve::Point& pt);
    std::ostream& operator<<(std::ostream& os, const EllipticCurve& pt);
    BigInt::KeyPair GenKeyPair(BigInt::EllipticCurve* curve);
    std::tuple<BigInt::KeyPair, BigInt::EllipticCurve::Point, BigInt::EllipticCurve::Point> encode(const BigInt::BigInteger &arg, BigInt::EllipticCurve& encoder);
    BigInt::BigInteger decode(const BigInt::EllipticCurve::Point &a,
                   const BigInt::EllipticCurve::Point &b,
                   const BigInt::KeyPair& keyPair);

}

#endif //BIGINTEGERLAB_BIGINTEGERALGORITHM_H

