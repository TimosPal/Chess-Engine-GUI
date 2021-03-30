#ifndef MOVETABLES_H
#define MOVETABLES_H

#include "../Board/Bitboard.h"

namespace ChessEngine::MoveTables {

    using namespace Bitboard_Util;

    // Access precalculated moves from compile time initialized arrays.

    Bitboard GetPrecalculated_PawnAttacks(Color color, uint8_t index);
    Bitboard GetPrecalculated_KnightMoves(uint8_t index);
    Bitboard GetPrecalculated_KingMoves(uint8_t index);

    Bitboard GetPrecalculated_RookAttacks(uint8_t index, Bitboard_Util::Bitboard occupancies);
    Bitboard GetPrecalculated_BishopAttacks(uint8_t index, Bitboard_Util::Bitboard occupancies);
    Bitboard GetPrecalculated_QueenAttacks(uint8_t index, Bitboard_Util::Bitboard occupancies);


}

#endif