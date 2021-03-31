#ifndef LEAPERPIECES_H
#define LEAPERPIECES_H

#include "../Board/Bitboard.h"

namespace ChessEngine::MoveGeneration::LeaperPieces {

    /*******************************************************/
    /* Pawn                                                */
    /*******************************************************/

    // When overflowing / underflowing in files A,H we may end up in the
    // opposite direction producing faulty moves. The inverted fileMasks
    // handle those 2 cases.

    /* Generate the attack moves of pawns */
    BitboardUtil::Bitboard GetPawnAttacks(BitboardUtil::Bitboard board, Color color);
    /* Generate the push moves of pawns */
    BitboardUtil::Bitboard GetPawnPushes(BitboardUtil::Bitboard board, Color color);
    /* Generate the double push moves of pawns , check for occupancy only on the first move. */
    BitboardUtil::Bitboard GetDoublePawnPushes(BitboardUtil::Bitboard board, BitboardUtil::Bitboard occupancies, Color color);

    /*******************************************************/
    /* Knight                                              */
    /*******************************************************/

    /* Generate the attack moves of all pawns at the given bitboard based on color */
    BitboardUtil::Bitboard GetKnightMoves(BitboardUtil::Bitboard board);

    /*******************************************************/
    /* King                                                */
    /*******************************************************/

    /* Generate the attack moves of all pawns at the given bitboard based on color */
    BitboardUtil::Bitboard GetKingMoves(BitboardUtil::Bitboard board);

}

#endif