#ifndef PSEUDO_MOVES_H
#define PSEUDO_MOVES_H

#include <list>

#include "../Board/BoardState.h"
#include "../Board/BoardOccupancies.h"
#include "Move.h"

namespace ChessEngine::MoveGeneration::Pseudo {

    /* The pseudo moves should be checked for validity afterwards. */
    std::list<Move> GetPseudoMoves(const BoardState &state, Color color, const BoardOccupancies& utilities);
    std::list<Move> GetPawnMoves(const BoardState &state, Color color, const BoardOccupancies& utilities);
    std::list<Move> GetKingMoves(const BoardState &state, Color color, const BoardOccupancies& utilities);
    std::list<Move> GetCastlingMoves(const BoardState &state, Color color, const BoardOccupancies& utilities);
    std::list<Move> GetKnightMoves(const BoardState &state, Color color, const BoardOccupancies& utilities);
    std::list<Move> GetSlidingMoves(const BoardState &state, Color color, PieceType type, const BoardOccupancies& utilities);
    std::list<Move> GetEnPassantMoves(const BoardState &state, Color color, const BoardOccupancies& utilities);

}

#endif
