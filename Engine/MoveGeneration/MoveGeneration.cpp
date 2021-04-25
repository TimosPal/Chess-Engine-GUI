#include "MoveGeneration.h"

#include <iostream>

namespace ChessEngine::MoveGeneration {

    using namespace BitboardUtil;

    void MakeMove(const Move& move, Color color, BoardState& state, BoardUtilities& utilities){
        // Update enemy bitboard.
        Color opponentColor = InvertColor(color);
        if(IsMoveType(move.flags, MoveType::Capture)) {
            Bitboard &enemyTypeBoard = state.pieceBoards[opponentColor][move.enemyType];
            enemyTypeBoard = PopBit(enemyTypeBoard, move.toSquareIndex);

            if (IsMoveType(move.flags, MoveType::EnPassant)) {
                // En passant is also a capture the attacking move has already been made.
                // Only delete the enpassant piece and reset state.
                Bitboard& pawnsBoard = state.pieceBoards[opponentColor][PieceType::Pawn];

                // EnPassantBoard has the pawn in front of the index
                // so we need to go up or down 1 square.
                Bitboard enPassantPiece = (opponentColor == Color::White) ?
                        ShiftUp(state.enPassantBoard) :
                        ShiftDown(state.enPassantBoard);
                uint8_t pawnIndex = GetLSBIndex(enPassantPiece);
                pawnsBoard = PopBit(pawnsBoard, pawnIndex);

                // Update square occupancies for said pawn and reset the state.
                utilities.squaresOccupants[pawnIndex] = {PieceType::None, Color::Both};
                state.enPassantBoard = BITBOARD_EMPTY;
            }
        }

        // Update self bitboard.
        // Enemy bit is already cleared so we swap 1 and 0.
        Bitboard& selfTypeBoard = state.pieceBoards[color][move.selfType];
        selfTypeBoard = SwapBit(selfTypeBoard, move.fromSquareIndex, move.toSquareIndex);

        // TODO: Castling

        // TODO: Promotions

        // Update squaresOccupants.
        // Doesn't catch en passant / rook on castling / promotions.
        utilities.squaresOccupants[move.fromSquareIndex] = {PieceType::None, Color::Both};
        utilities.squaresOccupants[move.toSquareIndex] = {move.selfType, color}; // TODO: doesnt work with promotions / castling

        // Update occupancies.
        Bitboard& selfOccupancies = utilities.occupancies[color];
        selfOccupancies = SwapBit(selfOccupancies, move.fromSquareIndex, move.toSquareIndex);

        Bitboard& enemyOccupancies = utilities.occupancies[opponentColor];
        enemyOccupancies = PopBit(enemyOccupancies, move.toSquareIndex);

        utilities.occupancies[Color::Both] = selfOccupancies | enemyOccupancies;

        // TODO: Counters

        // Update turn
        state.turnOf = opponentColor;
    }

    void PrintMoves(const std::list<Move>& moveList){
        for(auto move : moveList){
            std::cout << move << std::endl;
        }
    }

    bool IndecesToMove(uint8_t from, uint8_t to, const std::list<Move>& allMoves, Move& move){
        for (auto currMove : allMoves) {
            if (currMove.fromSquareIndex == from && currMove.toSquareIndex == to) {
                move = currMove;
                return true;
            }
        }

        return false;
    }

    std::list<Move> FromIndexMoves(uint8_t from, const std::list<Move>& moves){
        std::list<Move> fromMoves;

        for (auto currMove : moves) {
            if (currMove.fromSquareIndex == from) {
                fromMoves.push_back(currMove);
            }
        }

        return fromMoves;
    }

}