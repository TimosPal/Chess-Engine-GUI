#ifndef MOVE_TABLES_H
#define MOVE_TABLES_H

#include "../Board/Bitboard.h"
#include "MagicNumbers.h"

namespace ChessEngine::MoveGeneration::MoveTables {

    // Access precalculated moves from compile time initialized arrays.

    void InitMoveTables(); // Need to call at startup.

    BitboardUtil::Bitboard GetPawnAttacks(Color color, uint8_t index);
    BitboardUtil::Bitboard GetKnightMoves(uint8_t index);
    BitboardUtil::Bitboard GetKingMoves(uint8_t index);

    BitboardUtil::Bitboard GetRookMoves(uint8_t index, BitboardUtil::Bitboard occupancies);
    BitboardUtil::Bitboard GetBishopMoves(uint8_t index, BitboardUtil::Bitboard occupancies);
    BitboardUtil::Bitboard GetQueenMoves(uint8_t index, BitboardUtil::Bitboard occupancies);

    // TODO: change globals to static
    extern std::array<BitboardUtil::Bitboard, 64> kingMoves;
    extern std::array<BitboardUtil::Bitboard, 64> knightMoves;
    extern std::array<std::array<BitboardUtil::Bitboard, 64>, 2> pawnAttacks;
    extern std::array<BitboardUtil::Bitboard, MagicNumbers::permutations> slidingMoves;

}

#endif