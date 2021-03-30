#ifndef MAGICNUMBERS_H
#define MAGICNUMBERS_H

#include "../Board/Bitboard.h"

namespace ChessEngine::MagicNumbers {

    /* Produces a 64 bit random unsigned int */
    constexpr uint64_t Random64Bit() {
        uint64_t  u1, u2, u3, u4;
        u1 = (uint64_t)(rand()()) & 0xFFFF;
        u2 = (uint64_t)(rand()()) & 0xFFFF;
        u3 = (uint64_t)(rand()()) & 0xFFFF;
        u4 = (uint64_t)(rand()()) & 0xFFFF;
        return u1 | (u2 << 16) | (u3 << 32) | (u4 << 48);
    }

    /* Produces a good magic number candidate */
    constexpr uint64_t RandomMagicNumberCandidate() {
        return Random64Bit() & Random64Bit() & Random64Bit();
    }

}

#endif