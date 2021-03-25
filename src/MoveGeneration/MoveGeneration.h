#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include <cstdint>

#include "../Board/BoardState.h"

namespace ChessEngine::MoveGeneration {

    enum class MoveType {
        Quiet, Capture
    };

    struct Move {
        uint8_t fromSquareIndex: 3;
        uint8_t toSquareIndex: 3;
        MoveType type: 1;
    };

    void GenerateMoves(const BoardState &state, Color color, const Bitboard_Util::Bitboard* occupancies);

}

#endif
