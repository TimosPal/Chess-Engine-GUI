#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include <cstdint>

#include "../Board/BoardState.h"

namespace ChessEngine::MoveGeneration {

    enum MoveType {
        None = 0,
        Quiet = 1 << 0,
        Capture = 1 << 1,
        Promotion = 1 << 2
    };

    struct Move {
        uint8_t fromSquareIndex: 3;
        uint8_t toSquareIndex: 3;
        MoveType flags: 4;
    };


    void GenerateMoves(const BoardState &state, Color color, const Bitboard_Util::Bitboard* occupancies);

    bool IsOfMoveType(MoveType flags, MoveType type);
    std::string MoveTypeToString(MoveType type);
    std::ostream& operator<<(std::ostream& out, MoveType value);
    std::ostream& operator<<(std::ostream& out, Move value);
}

#endif
