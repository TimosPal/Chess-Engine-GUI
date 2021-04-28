#ifndef MOVE_H
#define MOVE_H

#include <cstdint>

#include "../Utilities/Utilities.h"

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

        PieceType selfType;
        PieceType enemyType;

        PieceType promotionType;
    };

    bool IsMoveType(MoveType flags, MoveType type);
    std::string MoveTypeToString(MoveType type); /* Should contain a single flag */

    std::ostream& operator<<(std::ostream& out, MoveType value);
    std::ostream& operator<<(std::ostream& out, Move value);
}

#endif
