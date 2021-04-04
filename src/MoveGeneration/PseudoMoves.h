#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include "../Board/BoardState.h"

namespace ChessEngine::MoveGeneration::Pseudo {

    /* The pseudo moves should be checked for validity afterwards. */
    void GetAllMoves(const BoardState &state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetPawnMoves(const BoardState& state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetKingMoves(const BoardState& state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetCastlingMoves(const BoardState& state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetKnightMoves(const BoardState& state, Color color, const BitboardUtil::Bitboard* occupancies);
    void GetSlidingMoves(const BoardState &state, Color color, const BitboardUtil::Bitboard *occupancies, PieceType type);

}

#endif
