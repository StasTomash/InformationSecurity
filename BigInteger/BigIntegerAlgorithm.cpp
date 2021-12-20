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

BigInt::BigInteger BigInt::InverseInCircle(const BigInt::BigInteger &arg, const BigInteger& modulo) {
    BigInteger x, y, g;
    g = BigInt::gcdExtended(arg, modulo, x, y);
    if (g != BigInteger(1)) {
        throw;
    }
    return (x + modulo) % modulo;
}

BigInt::KeyPair BigInt::GenKeyPair(BigInt::EllipticCurve* curve) {
    auto seed = BigInt::BigInteger::rand(curve->GetBase());
    auto basePoint = curve->createPoint();
    auto Q = basePoint * seed;
    return BigInt::KeyPair( {Q, seed } );
}

std::ostream &BigInt::operator<<(std::ostream &os, const BigInt::EllipticCurve::Point& pt) {
    os << "(" << pt.x << ", " << pt.y << ")";
    return os;
}

std::ostream &BigInt::operator<<(std::ostream &os, const BigInt::EllipticCurve &pt) {
    os << "a = " << pt.a << ", b = " << pt.b << ", p = " << pt.p;
    return os;
}

BigInt::EllipticCurve::Point
BigInt::EllipticCurve::createPoint(const BigInt::BigInteger &x, const BigInt::BigInteger &y) {
    return BigInt::EllipticCurve::Point(x, y, this);
}

BigInt::EllipticCurve::Point BigInt::EllipticCurve::createPoint() {
    return BigInt::EllipticCurve::Point(this);
}

BigInt::EllipticCurve::Point BigInt::EllipticCurve::Point::operator+(const BigInt::EllipticCurve::Point &other) const {
    Point ans(BigInteger(0), BigInteger(0), this->parent);
    if (this->x == other.x && this->y != other.y) {
        return ans;
    }
    if (this->x == BigInteger(0) && this->y == BigInteger(0)) {
        return other;
    }
    if (other.x == BigInteger(0) && other.y == BigInteger(0)) {
        return *this;
    }
    BigInteger slope;
    if (*this == other) {
        slope = BigInteger(3) * this->x * this->x + this->parent->a
                                                    * BigInt::InverseInCircle(BigInteger(2) * this->y , this->parent->p);
        slope %= this->parent->p;
    } else {
        slope = (this->y + this->parent->p - other.y)
                * BigInt::InverseInCircle(this->x + this->parent->p - other.x, this->parent->p);
        slope %= this->parent->p;
    }
    ans.x = (slope * slope + BigInteger(2) * this->parent->p - this->x - other.x) % this->parent->p;
    ans.y = (slope * (this->x - ans.x + this->parent->p) - this->y + this->parent->p) % this->parent->p;
    return ans;
}

bool BigInt::EllipticCurve::Point::operator==(const BigInt::EllipticCurve::Point& other) const {
    return (this->x == other.x && this->y == other.y);
}

BigInt::EllipticCurve::Point BigInt::EllipticCurve::Point::operator*(const BigInt::BigInteger &other) const {
    if (other == BigInteger(1)) {
        return *this;
    }
    if (other == BigInteger(0)) {
        return Point(BigInteger(0), BigInteger(0), this->parent);
    }
    Point ans(BigInteger(0), BigInteger(0), this->parent);
    Point cp(*this);
    BigInteger exp(other);
    while (exp != BigInteger(0)) {
        if (exp.isOdd()) {
            ans = ans + cp;
        }
        cp = cp + cp;
        exp /= BigInteger(2);
    }
    return ans;
}

BigInt::EllipticCurve::Point BigInt::EllipticCurve::Point::inverse() const {
    return BigInt::EllipticCurve::Point(x, parent->p - y, parent);
}

std::tuple<BigInt::KeyPair, BigInt::EllipticCurve::Point, BigInt::EllipticCurve::Point> BigInt::encode(const BigInt::BigInteger &arg, BigInt::EllipticCurve& encoder) {
    if (arg > BigInteger(255) || arg < BigInteger(0)) {
        throw BigInt::InvalidConsoleArguments();
    }

    auto seed = BigInteger::rand(encoder.GetBase());
    auto basePoint = encoder.createPoint();
    auto Q = basePoint * seed;

    auto keyPair = BigInt::KeyPair({Q, seed});

    auto sessionKey = BigInteger::rand(encoder.GetBase());
    auto M = Q * arg;
    auto a = basePoint * sessionKey;
    auto b = Q * sessionKey + M;

    auto ans = std::tuple{keyPair, a, b};
    return ans;
}

BigInt::BigInteger
BigInt::decode(const BigInt::EllipticCurve::Point &a,
               const BigInt::EllipticCurve::Point &b,
               const BigInt::KeyPair& keyPair) {
    auto M = b + (a * keyPair.privateKey).inverse();
    for (BigInteger m(0); m < BigInteger(256); m++) {
        if (keyPair.publicKey * m == M) {
            return m;
        }
    }
    return BigInteger(-1);
}
