//
// Created by stast on 10/31/2021.
//

#include <gtest/gtest.h>
#include "kupyna256.h"

TEST(Kupyna256Test, SmallGetHash) {
    Kupyna256 kupyna256{};
    std::string dataString = "The quick brown fox jumps over the lazy dog";
    std::vector<unsigned char> dataVector(dataString.begin(), dataString.end());
    auto hash = kupyna256.GetHash(dataVector);
    std::vector<unsigned int> expected({ 0x996899f2u, 0xd7422ceau, 0xf5524750u, 0x36b2dc12u,
                                            0x0607eff5u, 0x38abf2b8u, 0xdff471a9u, 0x8a4740c6u });
    ASSERT_EQ(hash, expected);
}