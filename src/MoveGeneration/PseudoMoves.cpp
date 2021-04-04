#include "PseudoMoves.h"

#include <list>
#include <iostream>
#include <cassert>

#include "MoveTables.h"
#include "LeaperPieces.h"
#include "Move.h"

namespace ChessEngine::MoveGeneration::Pseudo {

    using namespace ChessEngine::BitboardUtil;

    static std::list<Move> ExtractMoves(Bitboard moves, uint8_t fromSquareIndex, MoveType flags) {
        std::list<Move> moveList;

        // iterate over the bitboard , for each isolated bit find the corresponding moves.
        while (moves != 0) {
            uint8_t toSquareIndex = GetLSBIndex(moves);

            Move move = {.fromSquareIndex = fromSquareIndex,
                    .toSquareIndex = toSquareIndex,
                    .flags = flags};

            moveList.emplace_back(move);

            std::cout << move << std::endl;

            moves = PopBit(moves, toSquareIndex);
        }

        return moveList;
    }

    void GetPawnMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = occupancies[enemyColor];
        Bitboard globalOccupancies = occupancies[Color::Both];

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

            auto m1 = ExtractMoves(pushes & ~globalOccupancies, fromSquareIndex, quietMoveFlags);

            // Captures
            auto attackMoveFlags = (MoveType) (MoveType::Capture | promotionFlag);
            Bitboard attacks = MoveTables::GetPawnAttacks(color, fromSquareIndex);
            auto m2 = ExtractMoves(attacks & enemyOccupancies, fromSquareIndex, attackMoveFlags);

            pawnsBoard = PopBit(pawnsBoard, fromSquareIndex);
        }
    }

    void GetEnPassantMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
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
                std::cout << move << std::endl;

                enPassantPieces = PopBit(enPassantPieces, fromSquareIndex);
            }
        }
    }

    void GetCastlingMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        // Check whether or not there are pieces between the king and the rook.
        Bitboard globalOccupancies = occupancies[Color::Both];
        Bitboard colorMask = (color == Color::White) ? r1_Mask : r8_Mask;

        if (state.kingSideCastling[color]) {
            bool emptyKingSide = (colorMask & kingSideCastling_Mask & globalOccupancies) == 0;
            if (emptyKingSide) {
                Move move = {.fromSquareIndex = GetLSBIndex(kingsStartingPosBoard & colorMask),
                        .toSquareIndex = GetLSBIndex(kingsCastlePosBoard & colorMask),
                        .flags = MoveType::KingSideCastling};
                std::cout << move << std::endl;
            }
        }
        if (state.queenSideCastling[color]) {
            bool emptyQueenSide = (colorMask & queenSideCastling_Mask & globalOccupancies) == 0;
            if (emptyQueenSide) {
                Move move = {.fromSquareIndex = GetLSBIndex(kingsStartingPosBoard & colorMask),
                        .toSquareIndex = GetLSBIndex(queenCastlePosBoard & colorMask),
                        .flags = MoveType::QueenSideCastling};
                std::cout << move << std::endl;
            }
        }
    }

    void GetKnightMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = occupancies[enemyColor];
        Bitboard globalOccupancies = occupancies[Color::Both];

        // Pawns.
        Bitboard knightsBoard = state.pieceBoards[color][PieceType::Knight];
        while (knightsBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(knightsBoard);

            Bitboard moves = MoveTables::GetKnightMoves(fromSquareIndex);

            // Quiet moves
            auto m1 = ExtractMoves(moves & ~globalOccupancies, fromSquareIndex, MoveType::Quiet);

            // Captures
            auto m2 = ExtractMoves(moves & enemyOccupancies, fromSquareIndex, MoveType::Capture);

            knightsBoard = PopBit(knightsBoard, fromSquareIndex);
        }
    }

    void GetKingMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = occupancies[enemyColor];
        Bitboard globalOccupancies = occupancies[Color::Both];

        Bitboard kingBoard = state.pieceBoards[color][PieceType::King];

        if (kingBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(kingBoard);
            Bitboard moves = MoveTables::GetKingMoves(fromSquareIndex);

            // Quiet moves
            auto m1 = ExtractMoves(moves & ~globalOccupancies, fromSquareIndex, MoveType::Quiet);

            // Captures
            auto m2 = ExtractMoves(moves & enemyOccupancies, fromSquareIndex, MoveType::Capture);
        }
    }

    void GetSlidingMoves(const BoardState &state, Color color, const Bitboard *occupancies, PieceType type) {
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = occupancies[enemyColor];
        Bitboard globalOccupancies = occupancies[Color::Both];

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
            auto m1 = ExtractMoves(moves & ~globalOccupancies, fromSquareIndex, MoveType::Quiet);

            // Captures
            auto m2 = ExtractMoves(moves & enemyOccupancies, fromSquareIndex, MoveType::Capture);

            slidingPieceBoard = PopBit(slidingPieceBoard, fromSquareIndex);
        }
    }

    void GetAllMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        // Pawns.
        GetPawnMoves(state, color, occupancies);
        // King.
        GetKingMoves(state, color, occupancies);
        // Knight.
        GetKnightMoves(state, color, occupancies);
        // Castling.
        GetCastlingMoves(state, color, occupancies);
        // En passant.
        GetEnPassantMoves(state, color, occupancies);
        // Queen.
        GetSlidingMoves(state, color, occupancies, PieceType::Rook);
        // Bishop.
        GetSlidingMoves(state, color, occupancies, PieceType::Bishop);
        // Queen.
        GetSlidingMoves(state, color, occupancies, PieceType::Queen);
    }

}