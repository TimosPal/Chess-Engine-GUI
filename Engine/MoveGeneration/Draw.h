#ifndef DRAW_H
#define DRAW_H

#include "../Board/BoardState.h"

namespace ChessEngine::MoveGeneration::Draw {

    bool InsufficientMaterial(const BoardState& boardState);
    bool Stalemate();
    bool Repetition();
    bool MaxMoves();

}

#endif
