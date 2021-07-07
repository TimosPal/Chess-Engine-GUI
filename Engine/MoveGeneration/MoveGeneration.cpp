#include "MoveGeneration.h"

#include "Engine/MoveGeneration/MoveTables.h"
#include "Engine/MoveGeneration/PseudoMoves.h"
#include "Engine/MoveGeneration/Draw.h"

#include <iostream>
#include <cassert>

namespace ChessEngine::MoveGeneration {

    using namespace BitboardUtil;

    static void MakeMove_EnPassant(const Move &move,Color color, Color opponentColor, BoardState &state, BoardOccupancies &boardOccupancies) {
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

            // Update square boardOccupancies for said pawn
            boardOccupancies.squaresOccupants[pawnIndex] = {PieceType::None, Color::Both};

            // Update boardOccupancies board.
            Bitboard& enemyOccupancies = boardOccupancies.occupancies[opponentColor];
            enemyOccupancies = PopBit(enemyOccupancies, pawnIndex);

            // Reset en passant.
            state.enPassantBoard = BITBOARD_EMPTY;
        }
    }

    static void MakeMove_Castling(const Move &move,Color color, BoardState &state, BoardOccupancies &boardOccupancies){
        // The king has already moved , we need to handle the rook.

        // We know the rook has to go right of the king when castling queen side
        // and left of the king when castling king side.

        uint8_t rookNewIndex;
        bool kingSide;
        if(IsMoveType(move.flags, MoveType::QueenSideCastling)){
            rookNewIndex = move.toSquareIndex + 1;
            kingSide = false;
        }else{
            assert(IsMoveType(move.flags, MoveType::KingSideCastling));
            rookNewIndex = move.toSquareIndex - 1;
            kingSide = true;
        }
        uint8_t rookOldIndex = BitboardUtil::GetStartingRookIndex(color, kingSide);

        // Update rook piece board.
        Bitboard& rookBoard = state.pieceBoards[color][PieceType::Rook];
        rookBoard = PopBit(rookBoard, rookOldIndex);
        rookBoard = SetBit(rookBoard, rookNewIndex);

        // Update square boardOccupancies
        boardOccupancies.squaresOccupants[rookOldIndex] = {PieceType::None, Color::Both};
        boardOccupancies.squaresOccupants[rookNewIndex] = {PieceType::Rook, color};

        // Update boardOccupancies board.
        Bitboard& selfOccupancies = boardOccupancies.occupancies[color];
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

    void MakeMove(const Move& move, Color color, BoardState& state, BoardOccupancies& boardOccupancies){
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
            MakeMove_EnPassant(move, color , opponentColor, state, boardOccupancies);
        }else {
            // Reset en passant.
            state.enPassantBoard = BITBOARD_EMPTY;
        }

        // Castling.
        auto castlingFlags = (MoveType)(MoveType::QueenSideCastling | MoveType::KingSideCastling);
        if (IsMoveType(move.flags, castlingFlags)) {
            MakeMove_Castling(move, color, state, boardOccupancies);
        }

        // Check if any moves disabled any castling rights.
        // Castling is a king move so this is also caught here.
        DisableCastlingRights(move, color, opponentColor, state);

        // Update squaresOccupants.
        // Doesn't catch rook on castling / promotions.
        boardOccupancies.squaresOccupants[move.fromSquareIndex] = {PieceType::None, Color::Both};
        boardOccupancies.squaresOccupants[move.toSquareIndex] = {promotionType, color};

        // Update self boardOccupancies.
        Bitboard& selfOccupancies = boardOccupancies.occupancies[color];
        selfOccupancies = SwapBit(selfOccupancies, move.fromSquareIndex, move.toSquareIndex);

        // Update enemy boardOccupancies.
        // If not a capture , does nothing.
        Bitboard& enemyOccupancies = boardOccupancies.occupancies[opponentColor];
        enemyOccupancies = PopBit(enemyOccupancies, move.toSquareIndex);

        // Update global boardOccupancies.
        boardOccupancies.occupancies[Color::Both] = selfOccupancies | enemyOccupancies;

        // Update turn
        state.turnOf = opponentColor;
    }

    int NumberOfChecks(Color color, const BoardState& state, BoardOccupancies& boardOccupancies, uint8_t kingIndex){
        using namespace ChessEngine::MoveGeneration::MoveTables;
        using namespace ChessEngine::BitboardUtil;

        // We check if the king is attacked by enemy knights , bishops , rooks or pawns.
        // Since queens do bishop and rook attacks combined , this is accounted for by considering queens
        // as both piece types when calculating the appropriate piece occupancy boards.
        // If a king can attack an enemy piece with an attack pattern then that piece is causing a check.
        // Eg: if a king can attack a knight with a knight pattern move, then the king is checked by a knight.

        Color enemyColor = InvertColor(color);

        Bitboard occupancies = boardOccupancies.occupancies[Color::Both];

        Bitboard enemyQueenOCP = state.pieceBoards[enemyColor][PieceType::Queen];
        Bitboard enemyRookOCP = state.pieceBoards[enemyColor][PieceType::Rook] | enemyQueenOCP;
        Bitboard enemyBishopOCP = state.pieceBoards[enemyColor][PieceType::Bishop] | enemyQueenOCP;
        Bitboard enemyKnightOCP = state.pieceBoards[enemyColor][PieceType::Knight];
        Bitboard enemyPawnOCP = state.pieceBoards[enemyColor][PieceType::Pawn];
        Bitboard enemyKingOCP = state.pieceBoards[enemyColor][PieceType::King];

        Bitboard kingRookAttacks = GetRookMoves(kingIndex, occupancies);
        Bitboard kingBishopAttacks = GetBishopMoves(kingIndex, occupancies);
        Bitboard kingKnightAttacks = GetKnightMoves(kingIndex);
        Bitboard kingPawnAttacks = GetPawnAttacks(color, kingIndex);
        Bitboard kingKingAttacks = GetKingMoves(kingIndex);

        Bitboard attackSources =
                (kingRookAttacks & enemyRookOCP) |
                (kingBishopAttacks & enemyBishopOCP) |
                (kingKnightAttacks & enemyKnightOCP) |
                (kingPawnAttacks & enemyPawnOCP) |
                (kingKingAttacks & enemyKingOCP);

        return GetBitCount(attackSources);
    }

    int NumberOfChecks(Color color, const BoardState& state, BoardOccupancies& boardOccupancies){
        auto kingBoard = state.pieceBoards[color][PieceType::King];
        if(kingBoard) { // If not checked , an empty board will still give an index of 0.
            uint8_t kingIndex = GetLSBIndex(state.pieceBoards[color][PieceType::King]);
            return NumberOfChecks(color, state, boardOccupancies, kingIndex);
        }else{
            return 0; // Avoids invalid checks when king is missing in the board.
        }
    }

    bool IsValid(const Move& move, BoardState state, Color color, BoardOccupancies boardOccupancies) {
        bool isCastling = false;
        uint8_t intermediateIndexPos;
        if (IsMoveType(move.flags, MoveType::KingSideCastling)) {
            isCastling = true;
            intermediateIndexPos = move.toSquareIndex - 1;
        }
        if (IsMoveType(move.flags, MoveType::QueenSideCastling)) {
            isCastling = true;
            intermediateIndexPos = move.toSquareIndex + 1;
        }

        if(isCastling) {
            // For castling , we have to check 2 positions for checks.
            // since the king cant pass through a check.
            if (NumberOfChecks(color, state, boardOccupancies, intermediateIndexPos) != 0)
                return false;

            // We cant castle away from a check.
            if (NumberOfChecks(color, state, boardOccupancies, move.fromSquareIndex) != 0)
                return false;
        }

        // Play the move and check if the king is still in check.
        MakeMove(move, color, state, boardOccupancies);
        return NumberOfChecks(color, state, boardOccupancies) == 0;
    }

    std::list<Move> GetValidMoves(const BoardState& state, Color color, const BoardOccupancies& boardOccupancies){
        // TODO: possibly very slow to copy the board each time.
        auto pseudoMoves = ChessEngine::MoveGeneration::Pseudo::GetPseudoMoves(state, color, boardOccupancies);

        std::list<Move> validMoves;
        for(auto move : pseudoMoves){
            if(IsValid(move, state, color, boardOccupancies)){
                validMoves.push_back(move);
            }
        }

        return validMoves;
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