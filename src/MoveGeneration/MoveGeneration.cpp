#include "MoveGeneration.h"

#include <iostream>

namespace ChessEngine::MoveGeneration {

    using namespace BitboardUtil;

    void MakeMove(const Move& move, Color color, BoardState& state, BoardUtilities& utilities){
        // Update enemy bitboard.
        Color opponentColor = InvertColor(color);
        Bitboard& enemyTypeBoard = state.pieceBoards[opponentColor][move.enemyType];
        enemyTypeBoard = PopBit(enemyTypeBoard, move.toSquareIndex);

        // Update self bitboard.
        Bitboard& selfTypeBoard = state.pieceBoards[color][move.selfType];
        selfTypeBoard = SwapBit(selfTypeBoard, move.fromSquareIndex, move.toSquareIndex);

        // Update squaresOccupants.
        utilities.squaresOccupants[move.fromSquareIndex] = {PieceType::None, Color::Both};
        utilities.squaresOccupants[move.toSquareIndex] = {move.selfType, color};

        // Update occupancies.
        Bitboard& selfOccupancies = utilities.occupancies[color];
        selfOccupancies = SwapBit(selfOccupancies, move.fromSquareIndex, move.toSquareIndex);

        Bitboard& enemyOccupancies = utilities.occupancies[opponentColor];
        enemyOccupancies = PopBit(enemyOccupancies, move.toSquareIndex);

        utilities.occupancies[Color::Both] = selfOccupancies | enemyOccupancies;

        // TODO: Castling

        // TODO: Counters

        // Update turn
        state.turnOf = opponentColor;
    }

    void PrintMoves(const std::list<Move>& moveList){
        for(auto move : moveList){
            std::cout << move << std::endl;
        }
    }

}