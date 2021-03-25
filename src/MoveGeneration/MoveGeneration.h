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
        MoveType type: 2;
    };

    bool IsOfMoveType(MoveType flags, MoveType type);

    void GenerateMoves(const BoardState &state, Color color, const Bitboard_Util::Bitboard* occupancies);

}

#endif
