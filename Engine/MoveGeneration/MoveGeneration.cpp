#include "MoveGeneration.h"

#include <iostream>
#include <cassert>

namespace ChessEngine::MoveGeneration {

    using namespace BitboardUtil;

    static void MakeMove_EnPassant(const Move &move,Color color, Color opponentColor, BoardState &state, BoardUtilities &utilities) {
        // The en passant square is behind the pawn so we need to go
        // up or down 1 tile to find the appropriate position.
        // For this to work we assume the turn orders are correct.

        if (IsMoveType(move.flags, MoveType::Quiet)){
            // The move is already made , we just need to update the en passant state.
            Bitboard enPassantBoard = SetBit(BITBOARD_EMPTY, move.toSquareIndex);
            state.enPassantBoard = (color == Color::White) ?
                                      ShiftDown(enPassantBoard) :
                                      ShiftUp(enPassantBoard);
        }else{
            // En passant is also a capture but the enemy pawn isn't in the attacking
            // position so that should be taken account for.
            assert(IsMoveType(move.flags, MoveType::Capture));

            Bitboard enemyPawn = (opponentColor == Color::White) ?
                                 ShiftUp(state.enPassantBoard) :
                                 ShiftDown(state.enPassantBoard);
            uint8_t pawnIndex = GetLSBIndex(enemyPawn);

            Bitboard &enemyPawnBoard = state.pieceBoards[opponentColor][PieceType::Pawn];
            enemyPawnBoard = PopBit(enemyPawnBoard, pawnIndex);

            // Update square occupancies for said pawn
            utilities.squaresOccupants[pawnIndex] = {PieceType::None, Color::Both};

            Bitboard& enemyOccupancies = utilities.occupancies[opponentColor];
            enemyOccupancies = PopBit(enemyOccupancies, pawnIndex);

            // Reset en passant.
            state.enPassantBoard = BITBOARD_EMPTY;
        }
    }

    void MakeMove(const Move& move, Color color, BoardState& state, BoardUtilities& utilities){
        Color opponentColor = InvertColor(color);

        // Update self piece bitboard.
        Bitboard& selfTypeBoard = state.pieceBoards[color][move.selfType];
        selfTypeBoard = PopBit(selfTypeBoard, move.fromSquareIndex);
        // Promotion if available , otherwise just move.
        PieceType promotionType = (IsMoveType(move.flags, MoveType::Promotion)) ? PieceType::Queen : move.selfType;
        Bitboard& selfTypePromotionBoard = state.pieceBoards[color][promotionType];
        selfTypePromotionBoard = SetBit(selfTypePromotionBoard, move.toSquareIndex);

        // Update enemy piece board.
        if (IsMoveType(move.flags, MoveType::Capture)) {
            Bitboard &enemyTypeBoard = state.pieceBoards[opponentColor][move.enemyType];
            enemyTypeBoard = PopBit(enemyTypeBoard, move.toSquareIndex);
        }

        // EnPassant can mean either a capture or a double pawn move.
        if (IsMoveType(move.flags, MoveType::EnPassant)) {
            MakeMove_EnPassant(move,color , opponentColor, state, utilities);
        }else {
            // Reset en passant.
            state.enPassantBoard = BITBOARD_EMPTY;
        }

        // Update squaresOccupants.
        // Doesn't catch rook on castling / promotions.
        utilities.squaresOccupants[move.fromSquareIndex] = {PieceType::None, Color::Both};
        utilities.squaresOccupants[move.toSquareIndex] = {promotionType, color};

        // Update self occupancies.
        Bitboard& selfOccupancies = utilities.occupancies[color];
        selfOccupancies = SwapBit(selfOccupancies, move.fromSquareIndex, move.toSquareIndex);

        // Update enemy occupancies.
        // If not a capture , does nothing.
        Bitboard& enemyOccupancies = utilities.occupancies[opponentColor];
        enemyOccupancies = PopBit(enemyOccupancies, move.toSquareIndex);

        // Update global occupancies.
        utilities.occupancies[Color::Both] = selfOccupancies | enemyOccupancies;

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