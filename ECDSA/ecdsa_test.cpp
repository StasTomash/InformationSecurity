#pragma clang diagnostic push
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma ide diagnostic ignored "cert-err58-cpp"
#include <gtest/gtest.h>
#include "ecdsa.h"

TEST(ECDSA, TestEllipticCurveEncodeDecode) {
    BigInt::EllipticCurve curve;
    auto encoded = BigInt::encode(BigInt::BigInteger(10), curve);

    auto keyPair = std::get<0>(encoded);
    auto a = std::get<1>(encoded);
    auto b = std::get<2>(encoded);

    std::cout << "Public key is: \n" << keyPair.publicKey << std::endl;
    std::cout << "Private key is: \n" << keyPair.privateKey << std::endl;
    std::cout << "Encoded: \n";
    std::cout << "A = " << a << std::endl;
    std::cout << "B = " << b << std::endl;

    auto decoded = BigInt::decode(a, b, keyPair);
    ASSERT_EQ(decoded, BigInt::BigInteger(10));
}

TEST(ECDSA, TestCorrectSignature) {
    auto curve = new BigInt::EllipticCurve();

    std::cerr << "Generating Alice's key pair\n";
    auto aliceKeyPair = BigInt::GenKeyPair(curve);
    std::cerr << "Generating Bob's key pair\n";
    auto bobKeyPair = BigInt::GenKeyPair(curve);

    std::string aliceMsg = "Hello, I'm Alice";
    std::string bobMsg = "Hello Alice, I'm Bob";

    std::cerr << "Alice started signing message\n";
    auto aliceSignature = Sign(curve, aliceMsg, aliceKeyPair.privateKey);
    std::cerr << "Alice signed her message\n";
    std::cerr << "Bob verifying signature\n";
    ASSERT_TRUE(VerifySignature(curve, aliceSignature, aliceMsg, aliceKeyPair.publicKey));
    std::cerr << "Bob verified signature\n";

    std::cerr << "Bob started signing message\n";
    auto bobSignature = Sign(curve, bobMsg, bobKeyPair.privateKey);
    std::cerr << "Bob signed his message\n";
    std::cerr << "Alice verifying signature\n";
    ASSERT_TRUE(VerifySignature(curve, bobSignature, bobMsg, bobKeyPair.publicKey));
    std::cerr << "Alice verified signature\n";
}

TEST(ECDSA, TestIncorrectSignature) {
    auto curve = new BigInt::EllipticCurve();

    std::cerr << "Generating Alice's key pair\n";
    auto aliceKeyPair = BigInt::GenKeyPair(curve);
    std::cerr << "Generating Bob's key pair\n";
    auto bobKeyPair = BigInt::GenKeyPair(curve);

    std::string aliceMsg = "Hello, I'm Alice";
    std::string bobMsg = "Hello Alice, I'm Bob";

    std::cerr << "Alice started signing message\n";
    auto aliceSignature = Sign(curve, aliceMsg, bobKeyPair.privateKey);
    std::cerr << "Alice signed her message\n";
    std::cerr << "Bob verifying signature\n";
    ASSERT_FALSE(VerifySignature(curve, aliceSignature, aliceMsg, aliceKeyPair.publicKey));
    std::cerr << "Bob verified signature\n";

    std::cerr << "Bob started signing message\n";
    auto bobSignature = Sign(curve, bobMsg, aliceKeyPair.privateKey);
    std::cerr << "Bob signed his message\n";
    std::cerr << "Alice verifying signature\n";
    ASSERT_FALSE(VerifySignature(curve, bobSignature, bobMsg, bobKeyPair.publicKey));
    std::cerr << "Alice verified signature\n";
}