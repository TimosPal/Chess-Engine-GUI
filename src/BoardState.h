#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include "Bitboard.h"

namespace ChessEngine{

    struct BoardState{
        // A board with 6 bitboards for each color , 1 bitboard per unique piece type.
        // Use Color / PieceType for indexing
        Bitboard_Util::Bitboard pieceBoards[2][6];

        // TODO: castling rights , etc should be included here.
    };

}

#endif