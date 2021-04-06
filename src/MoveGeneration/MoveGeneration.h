#ifndef MOVE_GENERATION_H
#define MOVE_GENERATION_H

#include "Move.h"
#include "../Board/BoardState.h"
#include "../Board/BoardUtilities.h"

namespace ChessEngine::MoveGeneration {

    void MakeMove(const Move& move, Color color, BoardState& state, BoardUtilities& utilities){
        // Update enemy bitboard.
        Color opponentColor = InvertColor(color);
        BitboardUtil::Bitboard& enemyTypeBoard = state.pieceBoards[oponentColor][move.enemyType];
        enemyTypeBoard = BitboardUtil::PopBit(enemyTypeBoard, move.toSquareIndex);

        // Update self bitboard.
        BitboardUtil::Bitboard& selfTypeBoard = state.pieceBoards[color][move.selfType];
        selfTypeBoard = BitboardUtil::SwapBit(selfTypeBoard, move.fromSquareIndex, move.toSquareIndex);

        // Update squaresOccupants.
        utilities.squaresOccupants[move.fromSquareIndex] = MoveType::None;
        utilities.squaresOccupants[move.toSquareIndex] = move.selfType;

        // Update occupancies.
        BitboardUtil::Bitboard& selfOccupancies = utilities.occupancies[color];
        selfOccupancies = BitboardUtil::SwapBit(selfOccupancies, move.fromSquareIndex, move.toSquareIndex);

        BitboardUtil::Bitboard& enemyOccupancies = utilities.occupancies[oponentColor];
        enemyOccupancies = BitboardUtil::PopBit(enemyOccupancies, move.toSquareIndex);

        utilities.occupancies[Color::Both] = selfOccupancies | enemyOccupancies;

        // TODO: Castling

        // TODO: Counters

        // Update turn
        state.turnOf = opponentColor;
    }
}

#endif