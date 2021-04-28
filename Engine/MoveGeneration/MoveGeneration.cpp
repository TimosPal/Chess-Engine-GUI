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

            // Update occupancies board.
            Bitboard& enemyOccupancies = utilities.occupancies[opponentColor];
            enemyOccupancies = PopBit(enemyOccupancies, pawnIndex);

            // Reset en passant.
            state.enPassantBoard = BITBOARD_EMPTY;
        }
    }

    static void MakeMove_Castling(const Move &move,Color color, BoardState &state, BoardUtilities &utilities){
        // The king has already moved , we need to handle the rook.

        // We know the rook has to go right of the king when castling queen side
        // and left of the king when castling king side.

        uint8_t rookOldIndex;
        uint8_t rookNewIndex;
        Bitboard kingRankMask = (color == Color::White) ? r1_Mask : r8_Mask;
        if(IsMoveType(move.flags, MoveType::QueenSideCastling)){
            rookOldIndex = GetLSBIndex(BitboardUtil::queenRooksBoard & kingRankMask);
            uint8_t rightOfKing = move.toSquareIndex + 1;
            rookNewIndex = rightOfKing;
        }else{
            assert(IsMoveType(move.flags, MoveType::KingSideCastling));
            rookOldIndex = GetLSBIndex(BitboardUtil::kingRooksBoard & kingRankMask);
            uint8_t leftOfKing = move.toSquareIndex - 1;
            rookNewIndex = leftOfKing;
        }

        // Update rook piece board.
        Bitboard& rookBoard = state.pieceBoards[color][PieceType::Rook];
        rookBoard = PopBit(rookBoard, rookOldIndex);
        rookBoard = SetBit(rookBoard, rookNewIndex);

        // Update square occupancies
        utilities.squaresOccupants[rookOldIndex] = {PieceType::None, Color::Both};
        utilities.squaresOccupants[rookNewIndex] = {PieceType::Rook, color};

        // Update occupancies board.
        Bitboard& selfOccupancies = utilities.occupancies[color];
        selfOccupancies = PopBit(selfOccupancies, rookOldIndex);
        selfOccupancies = SetBit(selfOccupancies, rookNewIndex);
    }

    static void DisableCastling_OneSide(uint8_t posIndex, Color color, BoardState& state){
        // This is a helper function for DisableCastlingRights.
        // for checking if a rook was moved or captured.
        // (Based on the color and posIndex)
        Bitboard colorRankMask = (color == Color::White) ? r1_Mask : r8_Mask;
        auto posBoard = SetBit(BITBOARD_EMPTY, posIndex);
        if(posBoard & kingRooksBoard & colorRankMask){
            state.kingSideCastling[color] = false;
        }else if(posBoard & queenRooksBoard & colorRankMask){
            state.queenSideCastling[color] = false;
        }
    }

    static void DisableCastlingRights(const Move& move, Color color, Color opponentColor, BoardState& state){
        // Does nothing if nothing relevant to the castling rights happened.
        if(state.kingSideCastling[color] || state.queenSideCastling[color]) {
            if (move.selfType == PieceType::King) {
                state.kingSideCastling[color] = false;
                state.queenSideCastling[color] = false;
            } else if (move.selfType == PieceType::Rook) {
                // Check if own rooks moved.
                DisableCastling_OneSide(move.fromSquareIndex, color, state);
            }
        }

        if(state.kingSideCastling[opponentColor] || state.queenSideCastling[opponentColor]) {
            // Check if enemy rooks are captured.
            DisableCastling_OneSide(move.toSquareIndex, opponentColor, state);
        }
    }

    void MakeMove(const Move& move, Color color, BoardState& state, BoardUtilities& utilities){
        Color opponentColor = InvertColor(color);

        // Update self piece bitboard.
        Bitboard& selfTypeBoard = state.pieceBoards[color][move.selfType];
        selfTypeBoard = PopBit(selfTypeBoard, move.fromSquareIndex);
        // Promotion if available , otherwise just move the same piece.
        PieceType promotionType = (IsMoveType(move.flags, MoveType::Promotion)) ? move.promotionType : move.selfType;
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

        // Castling.
        auto castlingFlags = (MoveType)(MoveType::QueenSideCastling | MoveType::KingSideCastling);
        if (IsMoveType(move.flags, castlingFlags)) {
            MakeMove_Castling(move, color, state, utilities);
        }

        // Check if any moves disabled any castling rights.
        // Castling is a king move so this is also caught here.
        DisableCastlingRights(move, color, opponentColor, state);

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