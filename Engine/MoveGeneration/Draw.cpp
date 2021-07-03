#include "Draw.h"

namespace ChessEngine::MoveGeneration::Draw {

    bool InsufficientMaterial(const BoardState& boardState){
        using namespace ChessEngine::BitboardUtil;

        auto boards = boardState.pieceBoards;

        if(boards[Color::White][PieceType::Rook] != BITBOARD_EMPTY || boards[Color::Black][PieceType::Rook])
            return false;
        if(boards[Color::White][PieceType::Queen] != BITBOARD_EMPTY || boards[Color::Black][PieceType::Queen])
            return false;
        if(boards[Color::White][PieceType::Pawn] != BITBOARD_EMPTY || boards[Color::Black][PieceType::Pawn])
            return false;

        int whiteBishop = GetBitCount(boards[Color::White][PieceType::Bishop]);
        int blackBishop = GetBitCount(boards[Color::Black][PieceType::Bishop]);

        int whiteKnight = GetBitCount(boards[Color::White][PieceType::Knight]);
        int blackKnight = GetBitCount(boards[Color::Black][PieceType::Knight]);

        int total = whiteBishop + blackBishop + whiteKnight + blackKnight;

        // king versus king
        if(total == 0)
            return true;

        // king and bishop versus king
        if(total == 1 && (whiteBishop == 1 || blackBishop == 1))
            return true;

        // king and knight versus king
        if(total == 1 && (whiteKnight == 1 || blackKnight == 1))
            return true;

        // king and bishop versus king and bishop with the bishops on the same color.
        if(total == 2 && whiteBishop == blackBishop == 1) {
            // Check if same color.
            auto [wX, wY] = GetCoordinates(GetLSBIndex(boards[Color::White][PieceType::Bishop]));
            auto [bX, bY] = GetCoordinates(GetLSBIndex(boards[Color::Black][PieceType::Bishop]));
            return wX % 2 == wY % 2 == bX % 2 == bY % 2;
        }

        return false;
    }

}

