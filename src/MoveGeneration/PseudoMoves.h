#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include "../Board/BoardState.h"
#include "../Board/BoardUtilities.h"

namespace ChessEngine::MoveGeneration::Pseudo {

    /* The pseudo moves should be checked for validity afterwards. */
    void GetAllMoves(const BoardState &state, Color color, const BoardUtilities& utilities);
    void GetPawnMoves(const BoardState &state, Color color, const BoardUtilities& utilities);
    void GetKingMoves(const BoardState &state, Color color, const BoardUtilities& utilities);
    void GetCastlingMoves(const BoardState &state, Color color, const BoardUtilities& utilities);
    void GetKnightMoves(const BoardState &state, Color color, const BoardUtilities& utilities);
    void GetSlidingMoves(const BoardState &state, Color color, PieceType type, const BoardUtilities& utilities);
    void GetEnPassantMoves(const BoardState &state, Color color, const BoardUtilities& utilities);
}

#endif
