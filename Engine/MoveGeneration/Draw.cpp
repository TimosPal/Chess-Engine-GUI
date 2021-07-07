#include "Draw.h"

namespace ChessEngine::MoveGeneration::Draw {

    bool InsufficientMaterial(const BoardState& boardState) {
        using namespace ChessEngine::BitboardUtil;

        auto boards = boardState.pieceBoards;

        if (boards[Color::White][PieceType::Rook] || boards[Color::Black][PieceType::Rook])
            return false;
        if (boards[Color::White][PieceType::Queen] || boards[Color::Black][PieceType::Queen])
            return false;
        if (boards[Color::White][PieceType::Pawn] || boards[Color::Black][PieceType::Pawn])
            return false;

        int whiteBishop = GetBitCount(boards[Color::White][PieceType::Bishop]);
        int blackBishop = GetBitCount(boards[Color::Black][PieceType::Bishop]);

        int whiteKnight = GetBitCount(boards[Color::White][PieceType::Knight]);
        int blackKnight = GetBitCount(boards[Color::Black][PieceType::Knight]);

        int total = whiteBishop + blackBishop + whiteKnight + blackKnight;

        // king versus king , 1 bishop, 1 knight
        if (total <= 1) {
            return true;
        } else if (total == 2 && whiteBishop == blackBishop == 1) {
            // king and bishop versus king and bishop with the bishops on the same color.
            // Check if same color.
            auto[wX, wY] = GetCoordinates(GetLSBIndex(boards[Color::White][PieceType::Bishop]));
            auto[bX, bY] = GetCoordinates(GetLSBIndex(boards[Color::Black][PieceType::Bishop]));
            return wX % 2 == wY % 2 == bX % 2 == bY % 2;
        } else {
            return false;
        }
    }

    bool Stalemate(Board& board, const std::list<Move>& moves){ // TODO : maybe too slow.
        auto state = board.GetState();
        auto occupancies = board.GetOccupancies();
        return moves.empty() && NumberOfChecks(state.turnOf, state, occupancies) == 0;
    }

    bool IsDraw(Board& board, const std::list<Move>& moves){
        return Draw::InsufficientMaterial(board.GetState()) ||
               Draw::Stalemate(board, moves);
    }

    bool IsCheckmate(Board& board, const std::list<Move>& moves){ // TODO: move.
        return moves.empty() && NumberOfChecks(board.GetState().turnOf, board.GetState(), board.GetOccupancies());
    }

}

