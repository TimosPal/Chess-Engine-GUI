#ifndef DRAW_H
#define DRAW_H

#include <Engine/Board/Board.h>
#include "MoveGeneration.h"

namespace ChessEngine::MoveGeneration::Draw {

    bool InsufficientMaterial(const BoardState& boardState);
    bool Stalemate(Board& board, const std::list<Move>& moves);
    bool Repetition();
    bool MaxMoves();

    bool IsDraw(Board& board, const std::list<Move>& moves);
    bool IsCheckmate(Board& board, const std::list<Move>& moves);


}

#endif
