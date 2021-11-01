//
// Created by stast on 10/31/2021.
//

#include <gtest/gtest.h>
#include "kupyna256.h"

TEST(Kupyna256Test, PadInput) {
    std::vector<unsigned char> inputData({ 0xFFu } );
    std::vector<unsigned char> expectedPadding( { 0xFFu, 0x80u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u,
                                                 0x00u, 0x00u, 0x00u, 0x00u, 0x08u, 0x00u, 0x00u, 0x00u,
                                                 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u, 0x00u });
    PadCharData(inputData);
    ASSERT_EQ(inputData, expectedPadding);

    inputData.clear();
    inputData.reserve(64);
    for (int i = 0; i < 64; i++) {
        inputData.push_back(i);
    }
    expectedPadding.clear();
    expectedPadding.reserve(128);
    for (int i = 0; i < 64; i++) {
        expectedPadding.push_back(i);
    }
    expectedPadding.push_back(0x80u);
    for (int i = 65; i < 117; i++) {
        expectedPadding.push_back(0);
    }
    expectedPadding.push_back(0x02u);
    for (int i = 118; i < 128; i++) {
        expectedPadding.push_back(0);
    }
    PadCharData(inputData);
    ASSERT_EQ(inputData, expectedPadding);

}

TEST(Kupyna256Test, TransformationT1) {
    std::vector<unsigned char> inputData;
    inputData.reserve(64);
    for (int i = 0; i < 64; i++) {
        inputData.push_back(i);
    }

    std::vector<unsigned char> expectedResult( { 0x20u, 0xA0u, 0x66u, 0x01u, 0x6Cu, 0x8Du, 0xAAu, 0x5Au,
                                                  0xA2u, 0xACu, 0xA4u, 0x50u, 0xD2u, 0x1Fu, 0x27u, 0x96u,
                                                  0xFBu, 0xDCu, 0x2Eu, 0x0Cu, 0xC4u, 0x52u, 0xAFu, 0x0Au,
                                                  0xAFu, 0x67u, 0xE2u, 0x7Au, 0x07u, 0x55u, 0xCBu, 0x32u,
                                                  0x71u, 0x8Cu, 0x2Cu, 0x79u, 0x09u, 0x20u, 0x1Du, 0x3Eu,
                                                  0x7Au, 0x3Fu, 0x25u, 0x62u, 0x34u, 0xC8u, 0x0Bu, 0x70u,
                                                  0xD5u, 0x1Au, 0xE3u, 0x93u, 0x6Du, 0xB2u, 0x6Cu, 0xF5u,
                                                  0x6Eu, 0x1Fu, 0x1Bu, 0xA8u, 0xA0u, 0xA7u, 0xE1u, 0xC0u } );
    Kupyna256 kupyna256{};
    auto result = kupyna256.TransformationT1(inputData);
    ASSERT_EQ(result, expectedResult);
}

TEST(Kupyna256Test, TransformationT2) {
    std::vector<unsigned char> inputData;
    inputData.reserve(64);
    for (int i = 0; i < 64; i++) {
        inputData.push_back(i);
    }

    std::vector<unsigned char> expectedResult( { 0x2Du, 0x6Fu, 0x3Au, 0x8Eu, 0x12u, 0xF1u, 0x62u, 0xAEu,
                                                 0xC3u, 0xF7u, 0x6Eu, 0x04u, 0x02u, 0x57u, 0x50u, 0x68u,
                                                 0x67u, 0x18u, 0x24u, 0xEFu, 0x72u, 0xFEu, 0xA1u, 0xCDu,
                                                 0x7Du, 0x71u, 0xFDu, 0x4Du, 0x8Eu, 0x6Au, 0x27u, 0xA1u,
                                                 0x0Cu, 0x2Bu, 0xA7u, 0xEBu, 0xF3u, 0x1Cu, 0x27u, 0x7Fu,
                                                 0x91u, 0xDDu, 0x38u, 0x47u, 0x31u, 0x02u, 0x5Au, 0x8Du,
                                                 0xF3u, 0x01u, 0x30u, 0x49u, 0x27u, 0x9Cu, 0xF4u, 0x72u,
                                                 0x51u, 0xB2u, 0x43u, 0x4Fu, 0x26u, 0x32u, 0xF0u, 0x0Au } );
    Kupyna256 kupyna256{};
    auto result = kupyna256.TransformationT2(inputData);
    ASSERT_EQ(result, expectedResult);
}

TEST(Kupyna256Test, GetHash) {
    Kupyna256 kupyna256{};
    std::vector<unsigned char> inputData;
    inputData.reserve(64);
    for (int i = 0; i < 64; i++) {
        inputData.push_back(i);
    }
    auto hash = kupyna256.GetHash(inputData);
    std::vector<unsigned int> expected( { 0x08F4EE6Fu, 0x1BE6903Bu, 0x324C4E27u, 0x990CB24Eu,
                                             0xF69DD58Du, 0xBE84813Eu, 0xE0A52F66u, 0x31239875u });
    ASSERT_EQ(hash, expected);
}

TEST(Kupyna256Test, SmallGetHash) {
    Kupyna256 kupyna256{};
    std::string dataString = "The quick brown fox jumps over the lazy dog";
    std::vector<unsigned char> dataVector(dataString.begin(), dataString.end());
    auto hash = kupyna256.GetHash(dataVector);
    std::vector<unsigned int> expected({ 0x996899f2u, 0xd7422ceau, 0xf5524750u, 0x36b2dc12u,
                                            0x0607eff5u, 0x38abf2b8u, 0xdff471a9u, 0x8a4740c6u });
    ASSERT_EQ(hash, expected);
}