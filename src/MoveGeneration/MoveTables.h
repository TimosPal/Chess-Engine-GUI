#ifndef MOVETABLES_H
#define MOVETABLES_H

#include "../Board/Bitboard.h"

namespace ChessEngine::MoveTables {

    using namespace Bitboard_Util;

    // Access precalculated moves from compile time initialized arrays.

    Bitboard GetPawnAttacks(Color color, uint8_t index);
    Bitboard GetKnightMoves(uint8_t index);
    Bitboard GetKingMoves(uint8_t index);

    Bitboard GetRookMoves(uint8_t index, Bitboard_Util::Bitboard occupancies);
    Bitboard GetBishopMoves(uint8_t index, Bitboard_Util::Bitboard occupancies);
    Bitboard GetQueenMoves(uint8_t index, Bitboard_Util::Bitboard occupancies);


}

#endif