#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>

#define BYTE_MASK 0xffLU

namespace ChessEngine::Bitboard_Util {

    /* We are gonna represent our chess board with bitboards , a 64 bit
     * unsigned integer where each bit corresponds to a position on the board */
    using Bitboard = uint64_t;

    constexpr uint8_t boardSize = 8; // Size of the board , chess is 8x8.

    constexpr Bitboard GetRankMask(uint8_t rank) {
        return BYTE_MASK << (rank * boardSize);
    }

    constexpr std::array<Bitboard, boardSize> GetRankMasks() {
        std::array<Bitboard, boardSize> masks{};
        for (int rank = 0; rank < boardSize; rank++) {
            masks[rank] = GetRankMask(rank);
        }

        return masks;
    }

    /* Returns an unsigned 8 bit number with the corresponding rank from the given bitboard */
    constexpr uint8_t GetRank(const Bitboard_Util::Bitboard board, const uint8_t rank){
        // We don't explicitly mask the 8 ls bits sine we convert from uint64 to uint8
        return board >> (rank * boardSize);
    }

    constexpr auto rankMasks = GetRankMasks();

}

#endif