#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include <cstdint>

#include "BoardState.h"

namespace ChessEngine::MoveGeneration {

    struct Move{
        uint8_t fromSquareIndex : 3;
        uint8_t toSquareIndex : 3;
    };

    void GenerateAttacks(Bitboard_Util::Bitboard pieceBoard, Color color, Bitboard_Util::Bitboard enemyOccupancies);

}

#endif
