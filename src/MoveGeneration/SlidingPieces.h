#ifndef SLIDINGPIECES_H
#define SLIDINGPIECES_H

#include "../Board/Bitboard.h"

namespace ChessEngine::SlidingPieces {

    using namespace Bitboard_Util;

    // NOTE: performance isn't crucial for these functions
    // since they are run once when producing the tables.

    /*******************************************************/
    /* General                                             */
    /*******************************************************/

    constexpr Bitboard GetDirectionalMask(File file, Rank rank, int8_t dirX, int8_t dirY){
        Bitboard mask = BITBOARD_EMPTY;
        int x = file + dirX, y = rank + dirY;
        while(x > 0 && x < 7 && y > 0 && y < 7) {
            mask |= SetBit(BITBOARD_EMPTY, GetSquareIndex(x, y));
            x += dirX;
            y += dirY;
        }

        return mask;
    }

    constexpr Bitboard GetRookMask(File file, Rank rank){
        Bitboard mask = BITBOARD_EMPTY;

        mask |= GetDirectionalMask(file, rank, 0, 1);
        mask |= GetDirectionalMask(file, rank, 0, -1);
        mask |= GetDirectionalMask(file, rank, 1, 0);
        mask |= GetDirectionalMask(file, rank, -1, 0);

        // Exclude self.
        return mask;
    }

    constexpr Bitboard GetBishopMask(File file, Rank rank){
        Bitboard mask = BITBOARD_EMPTY;

        mask |= GetDirectionalMask(file, rank, 1, 1);
        mask |= GetDirectionalMask(file, rank, 1, -1);
        mask |= GetDirectionalMask(file, rank, -1, 1);
        mask |= GetDirectionalMask(file, rank, -1, -1);

        // Exclude self.
        return mask;
    }

    /* Generate a move table based on the given function for each board position. */
    constexpr std::array<Bitboard, 64> InitMasksTable(Bitboard getMask(File, Rank)) {
        std::array<Bitboard, 64> mask = {};

        for (uint8_t rank = 0; rank < 8; rank++) {
            for (uint8_t file = 0; file < 8; file++) {
                uint8_t squareIndex = GetSquareIndex(file, rank);
                mask[squareIndex] = getMask((File)file, (Rank)rank);
            }
        }

        return mask;
    }

    /*******************************************************/
    /* Attack masks                                        */
    /*******************************************************/

    constexpr std::array<Bitboard, 64> rookAttackMasks = InitMasksTable(GetRookMask);
    constexpr std::array<Bitboard, 64> bishopAttackMasks = InitMasksTable(GetBishopMask);


}

#endif