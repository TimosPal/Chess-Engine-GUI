#include "MoveGeneration.h"

#include <iostream>

namespace ChessEngine::MoveGeneration {

    using namespace BitboardUtil;
/*
    void MakeMove(const Move& move, Color color, BoardState& state, BoardUtilities& utilities){
        // Update enemy bitboard.
        Color opponentColor = InvertColor(color);
        if(IsMoveType(move.flags, MoveType::Capture)) {
            Bitboard &enemyTypeBoard = state.pieceBoards[opponentColor][move.enemyType];
            enemyTypeBoard = PopBit(enemyTypeBoard, move.toSquareIndex);

            if (IsMoveType(move.flags, MoveType::EnPassant)) {
                // En passant is also a capture but the enemy pawn isn't in the attacking
                // position so that should be taken account for.

                // EnPassantBoard has the pawn in front of the index
                // so we need to go up or down 1 square.
                // For this to work we assume the turn order / en passant position
                // are correct.
                Bitboard enPassantPiece = (opponentColor == Color::White) ?
                        ShiftUp(state.enPassantBoard) :
                        ShiftDown(state.enPassantBoard);
                uint8_t pawnIndex = GetLSBIndex(enPassantPiece);
                enemyTypeBoard = PopBit(enemyTypeBoard, pawnIndex);

                // Update square occupancies for said pawn
                utilities.squaresOccupants[pawnIndex] = {PieceType::None, Color::Both};

                Bitboard& enemyOccupancies = utilities.occupancies[opponentColor];
                enemyOccupancies = PopBit(enemyOccupancies, pawnIndex);
            }
        }

        // Update self bitboard.
        // Enemy bit is already 0 so the swap is safe.
        Bitboard& selfTypeBoard = state.pieceBoards[color][move.selfType];
        selfTypeBoard = SwapBit(selfTypeBoard, move.fromSquareIndex, move.toSquareIndex);

        // TODO: Castling

        // TODO: Promotions

        // Update squaresOccupants.
        // Doesn't catch rook on castling / promotions.
        utilities.squaresOccupants[move.fromSquareIndex] = {PieceType::None, Color::Both};
        utilities.squaresOccupants[move.toSquareIndex] = {move.selfType, color}; // TODO: doesnt work with promotions / castling

        // Update occupancies.
        Bitboard& selfOccupancies = utilities.occupancies[color];
        selfOccupancies = SwapBit(selfOccupancies, move.fromSquareIndex, move.toSquareIndex);

        Bitboard& enemyOccupancies = utilities.occupancies[opponentColor];
        enemyOccupancies = PopBit(enemyOccupancies, move.toSquareIndex);

        utilities.occupancies[Color::Both] = selfOccupancies | enemyOccupancies;

        // TODO: Counters

        // Reset en passant if current move isn't en passant
        if(IsMoveType(move.flags, MoveType::EnPassant) && IsMoveType(move.flags, MoveType::Quiet)) {

        }else{
            state.enPassantBoard = BITBOARD_EMPTY;
        }

        // Update turn
        state.turnOf = opponentColor;
    }
*/
    void MakeMove(const Move& move, Color color, BoardState& state, BoardUtilities& utilities){
        Color opponentColor = InvertColor(color);

        // Reset en passant.
        state.enPassantBoard = BITBOARD_EMPTY;

        // Update self piece bitboard.
        Bitboard& selfTypeBoard = state.pieceBoards[color][move.selfType];
        selfTypeBoard = PopBit(selfTypeBoard, move.fromSquareIndex);
        // Promotion if available , otherwise just move.
        PieceType promotionType = (IsMoveType(move.flags, MoveType::Promotion)) ? PieceType::Queen : move.selfType;
        Bitboard& selfTypePromotionBoard = state.pieceBoards[color][promotionType];
        selfTypePromotionBoard = SetBit(selfTypePromotionBoard, move.toSquareIndex);

        // Update enemy piece board.
        // If not a capture , does nothing.
        Bitboard &enemyTypeBoard = state.pieceBoards[opponentColor][move.enemyType];
        enemyTypeBoard = PopBit(enemyTypeBoard, move.toSquareIndex);

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