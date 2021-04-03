#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include <cstdint>

#include "../Board/BoardState.h"

namespace ChessEngine::MoveGeneration {

    enum MoveType {
        None = 0,
        Quiet = 1 << 0,
        Capture = 1 << 1,
        Promotion = 1 << 2,
        EnPassant = 1 << 3,
        KingSideCastling = 1 << 4,
        QueenSideCastling = 1 << 5
    };

    struct Move {
        uint8_t fromSquareIndex;
        uint8_t toSquareIndex;
        MoveType flags;
    };

    /* The pseudo moves should be checked for validity afterwards. */
    void GetPseudoMoves(const BoardState &state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetPseudoPawnMoves(const BoardState& state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetPseudoKingMoves(const BoardState& state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetPseudoCastlingMoves(const BoardState& state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetPseudoKnightMoves(const BoardState& state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetPseudoSlidingPieceMoves(const BoardState &state, Color color, const BitboardUtil::Bitboard *occupancies, PieceType type);

    bool IsOfMoveType(MoveType flags, MoveType type);
    std::string MoveTypeToString(MoveType type); /* Should contain a single flag */

    std::ostream& operator<<(std::ostream& out, MoveType value);
    std::ostream& operator<<(std::ostream& out, Move value);
}

#endif
