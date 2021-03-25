#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include "Bitboard.h"

namespace ChessEngine{

    struct BoardState{
        // A board with 6 bitboards for each color , 1 bitboard per unique piece type.
        // Use Color / PieceType for indexing
        Bitboard_Util::Bitboard pieceBoards[2][6];

        // Indicates whose turn is it.
        Color turnOf;

        // Castling rights. Index via color.
        bool queenSideCastling[2];
        bool kingSideCastling[2];

        // TODO: en pasant , turn counters.
    };

}

#endif