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

    constexpr Bitboard GetRookMask(File file, Rank rank){
        Bitboard mask = BITBOARD_EMPTY;
        for (int i = 1; i < 7; i++) {
            if(i == file)
                continue;
            mask |= SetBit(BITBOARD_EMPTY, GetSquareIndex(i, rank));
        }
        for (int i = 1; i < 7; i++) {
            if(i == rank)
                continue;
            mask |= SetBit(BITBOARD_EMPTY, GetSquareIndex(file, i));
        }

        return mask;
    }

    constexpr Bitboard GetBishopMask(File file, Rank rank){
        Bitboard mask = BITBOARD_EMPTY;

        return mask;
    }

    /* Generate a move table based on the given function for each board position. */
    constexpr std::array<Bitboard, 64> InitMasksTable(Bitboard getMask(File file, Rank rank)) {
        std::array<Bitboard, 64> mask = {};

        for (uint8_t rank = 0; rank < 8; rank++) {
            for (uint8_t file = 0; file < 8; file++) {
                uint8_t squareIndex = GetSquareIndex(file, rank);
                Bitboard board = SetBit(BITBOARD_EMPTY, GetSquareIndex(file, rank));
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