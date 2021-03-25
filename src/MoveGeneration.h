#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include <cstdint>

#include "BoardState.h"

namespace ChessEngine::MoveGeneration {

    enum class MoveType {
        Quiet, Capture
    };

    struct Move {
        uint8_t fromSquareIndex: 3;
        uint8_t toSquareIndex: 3;
        MoveType type: 1;
    };

    void GenerateMoves(BoardState &state, Color color, Bitboard_Util::Bitboard occupancies[2]);

}

#endif
