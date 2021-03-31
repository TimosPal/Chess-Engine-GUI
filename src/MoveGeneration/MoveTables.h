#ifndef MOVE_TABLES_H
#define MOVE_TABLES_H

#include "../Board/Bitboard.h"

namespace ChessEngine::MoveTables {

    // Access precalculated moves from compile time initialized arrays.

    BitboardUtil::Bitboard GetPawnAttacks(Color color, uint8_t index);
    BitboardUtil::Bitboard GetKnightMoves(uint8_t index);
    BitboardUtil::Bitboard GetKingMoves(uint8_t index);

    BitboardUtil::Bitboard GetRookMoves(uint8_t index, BitboardUtil::Bitboard occupancies);
    BitboardUtil::Bitboard GetBishopMoves(uint8_t index, BitboardUtil::Bitboard occupancies);
    BitboardUtil::Bitboard GetQueenMoves(uint8_t index, BitboardUtil::Bitboard occupancies);

}

#endif