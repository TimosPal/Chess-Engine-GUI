#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include <list>

#include "Move.h"
#include "../Board/BoardState.h"
#include "../Board/BoardUtilities.h"

namespace ChessEngine::MoveGeneration {

    void MakeMove(const Move& move, Color color, BoardState& state, BoardUtilities& utilities);

    void PrintMoves(const std::list<Move>& moveList);

}

#endif