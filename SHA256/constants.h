//
// Created by stast on 10/31/2021.
//

#ifndef INFORMATIONSECURITY_SHA256_CONSTANTS_H
#define INFORMATIONSECURITY_SHA256_CONSTANTS_H

const unsigned int initialH[8] = { 0x6A09E667u, 0xBB67AE85u, 0x3C6EF372u, 0xA54FF53Au,
                                   0x510E527Fu, 0x9B05688Cu, 0x1F83D9ABu, 0x5BE0CD19u};

const unsigned int k[64] = { 0x428A2F98u, 0x71374491u, 0xB5C0FBCFu, 0xE9B5DBA5u,
                             0x3956C25Bu, 0x59F111F1u, 0x923F82A4u, 0xAB1C5ED5u,
                             0xD807AA98u, 0x12835B01u, 0x243185BEu, 0x550C7DC3u,
                             0x72BE5D74u, 0x80DEB1FEu, 0x9BDC06A7u, 0xC19BF174u,
                             0xE49B69C1u, 0xEFBE4786u, 0x0FC19DC6u, 0x240CA1CCu,
                             0x2DE92C6Fu, 0x4A7484AAu, 0x5CB0A9DCu, 0x76F988DAu,
                             0x983E5152u, 0xA831C66Du, 0xB00327C8u, 0xBF597FC7u,
                             0xC6E00BF3u, 0xD5A79147u, 0x06CA6351u, 0x14292967u,
                             0x27B70A85u, 0x2E1B2138u, 0x4D2C6DFCu, 0x53380D13u,
                             0x650A7354u, 0x766A0ABBu, 0x81C2C92Eu, 0x92722C85u,
                             0xA2BFE8A1u, 0xA81A664Bu, 0xC24B8B70u, 0xC76C51A3u,
                             0xD192E819u, 0xD6990624u, 0xF40E3585u, 0x106AA070u,
                             0x19A4C116u, 0x1E376C08u, 0x2748774Cu, 0x34B0BCB5u,
                             0x391C0CB3u, 0x4ED8AA4Au, 0x5B9CCA4Fu, 0x682E6FF3u,
                             0x748F82EEu, 0x78A5636Fu, 0x84C87814u, 0x8CC70208u,
                             0x90BEFFFAu, 0xA4506CEBu, 0xBEF9A3F7u, 0xC67178F2u };

#endif //INFORMATIONSECURITY_SHA256_CONSTANTS_H
