#include "PseudoMoves.h"

#include <iostream>
#include <cassert>

#include "MoveTables.h"
#include "LeaperPieces.h"

namespace ChessEngine::MoveGeneration::Pseudo {

    using namespace ChessEngine::BitboardUtil;

    static std::list<Move> ExtractMoves(Bitboard moves, uint8_t fromSquareIndex, MoveType flags, const BoardUtilities& utilities) {
        std::list<Move> moveList;

        // iterate over the bitboard , for each isolated bit find the corresponding moves.
        while (moves != 0) {
            uint8_t toSquareIndex = GetLSBIndex(moves);

            auto [selfType, selfColor] = utilities.squaresOccupants[fromSquareIndex];
            auto [enemyType, enemyColor] = utilities.squaresOccupants[toSquareIndex];

            Move move = {
                    .fromSquareIndex = fromSquareIndex,
                    .toSquareIndex = toSquareIndex,
                    .flags = flags,
                    .selfType = selfType,
                    .enemyType = enemyType,
            };

            moveList.emplace_back(move);

            moves = PopBit(moves, toSquareIndex);
        }

        return moveList;
    }

    std::list<Move> GetPawnMoves(const BoardState &state, Color color, const BoardUtilities& utilities) {
        std::list<Move> moveList;
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = utilities.occupancies[enemyColor];
        Bitboard globalOccupancies = utilities.occupancies[Color::Both];

        // Pawns.
        Bitboard pawnsBoard = state.pieceBoards[color][PieceType::Pawn];
        while (pawnsBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(pawnsBoard);
            Bitboard tempPieceBoard = SetBit(BITBOARD_EMPTY, fromSquareIndex);

            // Promotion Flag.
            // If the pawn is 1 tile away , moving or
            // attacking will lead to a promotion
            MoveType promotionFlag = MoveType::None;
            if ((color == Color::White && tempPieceBoard & r7_Mask) ||
                (color == Color::Black && tempPieceBoard & r2_Mask)) {
                promotionFlag = MoveType::Promotion;
            }

            // Quiet moves
            auto quietMoveFlags = (MoveType) (MoveType::Quiet | promotionFlag);
            Bitboard pushes = LeaperPieces::GetPawnPushes(tempPieceBoard, color);
            pushes |= LeaperPieces::GetDoublePawnPushes(tempPieceBoard, globalOccupancies, color);

            auto quietMoves = ExtractMoves(pushes & ~globalOccupancies, fromSquareIndex, quietMoveFlags, utilities);
            moveList.splice(moveList.end(), quietMoves);

            // Captures
            auto attackMoveFlags = (MoveType) (MoveType::Capture | promotionFlag);
            Bitboard attacks = MoveTables::GetPawnAttacks(color, fromSquareIndex);
            auto captures = ExtractMoves(attacks & enemyOccupancies, fromSquareIndex, attackMoveFlags, utilities);
            moveList.splice(moveList.end(), captures);

            pawnsBoard = PopBit(pawnsBoard, fromSquareIndex);
        }

        return moveList;
    }

    std::list<Move> GetEnPassantMoves(const BoardState &state, Color color, const BoardUtilities& utilities) {
        std::list<Move> moveList;
        Color enemyColor = InvertColor(color);

        uint8_t enPassantIndex = GetLSBIndex(state.enPassantBoard);
        if (state.enPassantBoard != BITBOARD_EMPTY) {
            Color enPassantColor = state.enPassantBoard & r3_Mask ? Color::White : Color::Black;

            // Enemy prev turn must have caused en passant!
            assert(enemyColor == enPassantColor);

            // Only possible attackers are the en passant's 2 corners , as if it were an attacking pawn.
            Bitboard enPassantPieces = MoveTables::GetPawnAttacks(enPassantColor, enPassantIndex);
            enPassantPieces &= state.pieceBoards[color][PieceType::Pawn]; // check only pawn attackers.
            while(enPassantPieces != 0){ // Max 2 loops.
                uint8_t fromSquareIndex = GetLSBIndex(enPassantPieces);

                auto enPassantMoveFlags = (MoveType) (MoveType::Capture | MoveType::EnPassant);
                Move move = {.fromSquareIndex = fromSquareIndex,
                             .toSquareIndex = enPassantIndex,
                             .flags = enPassantMoveFlags};

                moveList.push_back(move);

                enPassantPieces = PopBit(enPassantPieces, fromSquareIndex);
            }
        }

        return moveList;
    }

    std::list<Move> GetCastlingMoves(const BoardState &state, Color color, const BoardUtilities& utilities) {
        std::list<Move> moveList;
        // Check whether or not there are pieces between the king and the rook.
        Bitboard globalOccupancies = utilities.occupancies[Color::Both];
        Bitboard colorMask = (color == Color::White) ? r1_Mask : r8_Mask;

        if (state.kingSideCastling[color]) {
            bool emptyKingSide = (colorMask & kingSideCastling_Mask & globalOccupancies) == 0;
            if (emptyKingSide) {
                Move move = {.fromSquareIndex = GetLSBIndex(kingsStartingPosBoard & colorMask),
                        .toSquareIndex = GetLSBIndex(kingsCastlePosBoard & colorMask),
                        .flags = MoveType::KingSideCastling};

                moveList.push_back(move);
            }
        }
        if (state.queenSideCastling[color]) {
            bool emptyQueenSide = (colorMask & queenSideCastling_Mask & globalOccupancies) == 0;
            if (emptyQueenSide) {
                Move move = {.fromSquareIndex = GetLSBIndex(kingsStartingPosBoard & colorMask),
                        .toSquareIndex = GetLSBIndex(queenCastlePosBoard & colorMask),
                        .flags = MoveType::QueenSideCastling};

                moveList.push_back(move);
            }
        }

        return moveList;
    }

    std::list<Move> GetKnightMoves(const BoardState &state, Color color, const BoardUtilities& utilities) {
        std::list<Move> moveList;
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = utilities.occupancies[enemyColor];
        Bitboard globalOccupancies = utilities.occupancies[Color::Both];

        // Pawns.
        Bitboard knightsBoard = state.pieceBoards[color][PieceType::Knight];
        while (knightsBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(knightsBoard);

            Bitboard moves = MoveTables::GetKnightMoves(fromSquareIndex);

            // Quiet moves
            auto quietMoves = ExtractMoves(moves & ~globalOccupancies, fromSquareIndex, MoveType::Quiet, utilities);
            moveList.splice(moveList.end(), quietMoves);

            // Captures
            auto captures = ExtractMoves(moves & enemyOccupancies, fromSquareIndex, MoveType::Capture, utilities);
            moveList.splice(moveList.end(), captures);


            knightsBoard = PopBit(knightsBoard, fromSquareIndex);
        }

        return moveList;
    }

    std::list<Move> GetKingMoves(const BoardState &state, Color color, const BoardUtilities& utilities) {
        std::list<Move> moveList;
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = utilities.occupancies[enemyColor];
        Bitboard globalOccupancies = utilities.occupancies[Color::Both];

        Bitboard kingBoard = state.pieceBoards[color][PieceType::King];

        if (kingBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(kingBoard);
            Bitboard moves = MoveTables::GetKingMoves(fromSquareIndex);

            // Quiet moves
            auto quietMoves = ExtractMoves(moves & ~globalOccupancies, fromSquareIndex, MoveType::Quiet, utilities);
            moveList.splice(moveList.end(), quietMoves);

            // Captures
            auto captures = ExtractMoves(moves & enemyOccupancies, fromSquareIndex, MoveType::Capture, utilities);
            moveList.splice(moveList.end(), captures);

        }

        return moveList;
    }

    std::list<Move> GetSlidingMoves(const BoardState &state, Color color, PieceType type, const BoardUtilities& utilities) {
        std::list<Move> moveList;
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = utilities.occupancies[enemyColor];
        Bitboard globalOccupancies = utilities.occupancies[Color::Both];

        Bitboard slidingPieceBoard = state.pieceBoards[color][type];

        Bitboard (*getMoves)(uint8_t, Bitboard);
        switch (type) { // Find proper function for piece type.
            case PieceType::Queen: getMoves = MoveTables::GetQueenMoves; break;
            case PieceType::Rook: getMoves = MoveTables::GetRookMoves; break;
            case PieceType::Bishop: getMoves = MoveTables::GetBishopMoves; break;
            default: assert(false); // Invalid
        }

        while (slidingPieceBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(slidingPieceBoard);

            Bitboard moves = getMoves(fromSquareIndex, globalOccupancies);

            // Quiet moves
            auto quietMoves = ExtractMoves(moves & ~globalOccupancies, fromSquareIndex, MoveType::Quiet, utilities);
            moveList.splice(moveList.end(), quietMoves);

            // Captures
            auto captures = ExtractMoves(moves & enemyOccupancies, fromSquareIndex, MoveType::Capture, utilities);
            moveList.splice(moveList.end(), captures);

            slidingPieceBoard = PopBit(slidingPieceBoard, fromSquareIndex);
        }

        return moveList;
    }

    std::list<Move> GetAllMoves(const BoardState &state, Color color, const BoardUtilities& utilities) {
        std::list<Move> moveList;

        // Pawns.
        auto pawnMoves = GetPawnMoves(state, color, utilities);
        moveList.splice(moveList.end(), pawnMoves);
        // King.
        auto kingMoves = GetKingMoves(state, color, utilities);
        moveList.splice(moveList.end(), kingMoves);
        // Knight.
        auto knightMoves = GetKnightMoves(state, color, utilities);
        moveList.splice(moveList.end(), knightMoves);
        // Castling.
        auto castlingMoves = GetCastlingMoves(state, color, utilities);
        moveList.splice(moveList.end(), castlingMoves);
        // En passant.
        auto enPassantMoves = GetEnPassantMoves(state, color, utilities);
        moveList.splice(moveList.end(), enPassantMoves);
        // rook.
        auto rookMoves = GetSlidingMoves(state, color, PieceType::Rook, utilities);
        moveList.splice(moveList.end(), rookMoves);
        // Bishop.
        auto bishopMoves = GetSlidingMoves(state, color, PieceType::Bishop, utilities);
        moveList.splice(moveList.end(), bishopMoves);
        // Queen.
        auto queenMoves = GetSlidingMoves(state, color, PieceType::Queen, utilities);
        moveList.splice(moveList.end(), queenMoves);

        return moveList;
    }

}