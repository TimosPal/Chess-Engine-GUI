#ifndef BOARDSTATE_H
#define BOARDSTATE_H

#include "Bitboard.h"

namespace ChessEngine{

    struct BoardState{
        // A board with 6 bitboards for each color , 1 bitboard per unique piece type.
        // Use Color / PieceType for indexing
        Bitboard_Util::Bitboard pieceBoards[2][6]{};

        // Indicates whose turn is it.
        Color turnOf = Color::White;

        // Castling rights. Index via color.
        bool queenSideCastling[2]{};
        bool kingSideCastling[2]{};

        // Half moves helps the 50 move rule.
        // Full moves should start at 1 and is updated every Black's turn.
        int halfMoves = 0;
        int fullMoves = 1;

        // TODO: en pasant
        // En passant
        // Represents the position behind the pawn.
        Bitboard_Util::Bitboard enPassantBoard = 0;

    };

}

#endif