#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include <list>

#include "Move.h"
#include "../Board/BoardState.h"
#include "../Board/BoardUtilities.h"

namespace ChessEngine::MoveGeneration {

    void MakeMove(const Move& move, Color color, BoardState& state, BoardUtilities& utilities);

    int NumberOfChecks(Color color, const BoardState& state, BoardUtilities& utilities);

    std::list<Move> GetValidMoves(const BoardState& state, Color color, const BoardUtilities& utilities);

    void PrintMoves(const std::list<Move>& moveList);

    // Find the move with the specific from and to indices.
    bool IndecesToMove(uint8_t from, uint8_t to, const std::list<Move>& allMoves, Move& move);

    std::list<Move> FromIndexMoves(uint8_t from, const std::list<Move>& moves);

}

#endif