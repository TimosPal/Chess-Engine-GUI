#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include "Bitboard.h"

namespace ChessEngine{

    struct BoardState{
        // A board with 6 bitboards for each color , 1 bitboard per unique piece flags.
        // Use Color / PieceType for indexing
        BitboardUtil::Bitboard pieceBoards[2][6]{};

        // Indicates whose turn is it.
        Color turnOf = Color::White;

        // Castling rights. Index via color.
        bool queenSideCastling[2]{};
        bool kingSideCastling[2]{};

        // Half moves helps the 50 move rule.
        // Full moves should start at 1 and is updated every Black's turn.
        int halfMoves = 0;
        int fullMoves = 1;

        // En passant
        // Represents the position behind the pawn.
        // Is EMPTY when there is no en passant available
        BitboardUtil::Bitboard enPassantBoard = BITBOARD_EMPTY;

        std::tuple<PieceType, Color> GetPosType(uint8_t index){
            for (uint8_t i = 0; i < 12; i++) {
                uint8_t pieceIndex = i % 6;
                Color color = i < 6 ? Color::White : Color::Black;

                BitboardUtil::Bitboard currBoard = pieceBoards[color][pieceIndex];
                if(BitboardUtil::GetBit(currBoard, index)){
                    return {(PieceType)pieceIndex, color};
                }
            }

            return {PieceType::None, Color::Both};
        }

    };

}

#endif