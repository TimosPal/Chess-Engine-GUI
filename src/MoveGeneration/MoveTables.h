#ifndef MOVETABLES_H
#define MOVETABLES_H

#include "../Board/Bitboard.h"

namespace ChessEngine::MoveTables {

    using namespace Bitboard_Util;

    // Access precalculated moves from compile time initialized arrays.

    Bitboard GetPrecalculated_PawnAttacks(Color color, uint8_t index);
    Bitboard GetPrecalculated_KnightMoves(uint8_t index);
    Bitboard GetPrecalculated_KingMoves(uint8_t index);

}

#endif