#ifndef SLIDINGPIECES_H
#define SLIDINGPIECES_H

#include "../Board/Bitboard.h"

namespace ChessEngine::LeaperPieces {

    using namespace Bitboard_Util;

    // NOTE: performance isn't crucial for these functions
    // since they are run once when producing the tables.

    /*******************************************************/
    /* Rook                                                */
    /*******************************************************/

    constexpr std::array<Bitboard, 64> kingMoves = InitMoveTable(GetKingMoves);


}

#endif