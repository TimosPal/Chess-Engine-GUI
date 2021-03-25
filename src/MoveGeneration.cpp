#include "MoveGeneration.h"

#include <list>

#include "NonSlidingPieces.h"

using namespace ChessEngine;
using namespace ChessEngine::Bitboard_Util;
using namespace ChessEngine::MoveGeneration;

static std::list<Move> ExtractMoves(Bitboard moves, uint8_t fromSquareIndex, MoveType type){
    std::list<Move> moveList;

    // iterate over the bitboard , for each isolated bit find the corresponding moves.
    while(moves != 0){
        uint8_t toSquareIndex = GetLSBIndex(moves);

        Move move = {.fromSquareIndex = fromSquareIndex,
                     .toSquareIndex = toSquareIndex,
                     .type = type};
        moveList.emplace_back(move);

        moves = PopBit(moves, toSquareIndex);
    }

    return moveList;
}

void ChessEngine::MoveGeneration::GenerateMoves(BoardState& state, Color color, Bitboard_Util::Bitboard occupancies[2]) {
    Color enemyColor = InvertColor(color);

    // Pawns.
    Bitboard pawnsBoard = state.pieceBoards[color][PieceType::Pawn];
    while(pawnsBoard != 0){
        uint8_t fromSquareIndex = GetLSBIndex(pawnsBoard);

        // Quiet moves

        // Captures
        Bitboard attacks = NonSlidingPieces::pawnAttacks[color][fromSquareIndex];
        auto m = ExtractMoves(attacks & occupancies[enemyColor], fromSquareIndex, MoveType::Capture);

        pawnsBoard = PopBit(pawnsBoard, fromSquareIndex);
    }

    // Knight / King.


    // Queen / rook / bishop.


}