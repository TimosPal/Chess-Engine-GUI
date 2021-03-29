#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

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
    void GeneratePseudoMoves(const BoardState &state, Color color, const Bitboard_Util::Bitboard* occupancies);
    void GeneratePseudoPawnMoves(const BoardState& state, Color color, const Bitboard_Util::Bitboard* occupancies);
    void GeneratePseudoKingMoves(const BoardState& state, Color color, const Bitboard_Util::Bitboard* occupancies);
    void GeneratePseudoCastlingMoves(const BoardState& state, Color color, const Bitboard_Util::Bitboard* occupancies);
    void GeneratePseudoKnightMoves(const BoardState& state, Color color, const Bitboard_Util::Bitboard* occupancies);

    bool IsOfMoveType(MoveType flags, MoveType type);
    std::string MoveTypeToString(MoveType type); /* Should contain a single flag */

    std::ostream& operator<<(std::ostream& out, MoveType value);
    std::ostream& operator<<(std::ostream& out, Move value);
}

#endif
