//
// Created by stast on 11/28/2021.
//

#ifndef INFORMATIONSECURITY_ECDSA_H
#define INFORMATIONSECURITY_ECDSA_H

#include "../BigInteger/BigIntegerAlgorithm.h"

std::pair<BigInt::BigInteger, BigInt::BigInteger> Sign(
    BigInt::EllipticCurve* curve,
    const std::string& msg,
    const BigInt::BigInteger& privateKey
) {
    BigInt::BigInteger h = BigInt::BigInteger(std::hash<std::string>()(msg));
    while (true) {
        BigInt::BigInteger k = BigInt::BigInteger::rand(curve->GetGeneratorOrder());
        BigInt::EllipticCurve::Point p = BigInt::EllipticCurve::Point(curve) * k;
        BigInt::BigInteger r = p.GetX() % curve->GetGeneratorOrder();
        if (r == BigInt::BigInteger(0)) {
            continue;
        }
        BigInt::BigInteger s = (BigInt::InverseInCircle(k, curve->GetGeneratorOrder()) * (h + r * privateKey)) % curve->GetGeneratorOrder();
        if (s == BigInt::BigInteger(0)) {
            continue;
        }
        return { r, s };
    }
}

bool VerifySignature(
    BigInt::EllipticCurve* curve,
    const std::pair<BigInt::BigInteger, BigInt::BigInteger>& signature,
    const std::string& msg,
    const BigInt::EllipticCurve::Point& publicKey
) {
    BigInt::BigInteger r = signature.first, s = signature.second;
    BigInt::BigInteger h = BigInt::BigInteger(std::hash<std::string>()(msg));
    BigInt::BigInteger inv = BigInt::InverseInCircle(s, curve->GetGeneratorOrder());
    BigInt::BigInteger u1 = (h * inv) % curve->GetGeneratorOrder();
    BigInt::BigInteger u2 = (r * inv) % curve->GetGeneratorOrder();
    BigInt::EllipticCurve::Point p = BigInt::EllipticCurve::Point(curve) * u1 + publicKey * u2;
    return (r % curve->GetGeneratorOrder() == p.GetX() % curve->GetGeneratorOrder());
}

#endif //INFORMATIONSECURITY_ECDSA_H
