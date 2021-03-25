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

        auto [xf , yf] = GetCoordinates(fromSquareIndex);
        auto [xt , yt] = GetCoordinates(toSquareIndex);

        printf("%s [%s%s]->[%s%s]\n",
               (type == MoveType::Capture ? "Capture" : "Move"),
               FileToString((File)xf).c_str(),
               RankToString((Rank)yf).c_str(),
               FileToString((File)xt).c_str(),
               RankToString((Rank)yt).c_str());

        moves = PopBit(moves, toSquareIndex);
    }

    return moveList;
}

void ChessEngine::MoveGeneration::GenerateMoves(const BoardState& state, Color color, const Bitboard_Util::Bitboard* occupancies) {
    Color enemyColor = InvertColor(color);
    Bitboard teamOccupancies = occupancies[color];
    Bitboard enemyOccupancies = occupancies[enemyColor];
    Bitboard globalOccupancies = occupancies[Color::Both];

    // Pawns.
    Bitboard pawnsBoard = state.pieceBoards[color][PieceType::Pawn];
    while(pawnsBoard != 0){
        uint8_t fromSquareIndex = GetLSBIndex(pawnsBoard);
        Bitboard tempPieceBoard = SetBit(BITBOARD_EMPTY, fromSquareIndex);

        // Quiet moves
        Bitboard pushes = NonSlidingPieces::GetPawnPushes(tempPieceBoard, color);
        pushes |= NonSlidingPieces::GetDoublePawnPushes(tempPieceBoard, globalOccupancies, color);
        auto m1 = ExtractMoves(pushes & ~globalOccupancies, fromSquareIndex, MoveType::Quiet);

        // Captures
        Bitboard attacks = NonSlidingPieces::pawnAttacks[color][fromSquareIndex];
        auto m2 = ExtractMoves(attacks & enemyOccupancies, fromSquareIndex, MoveType::Capture);

        pawnsBoard = PopBit(pawnsBoard, fromSquareIndex);
    }

    // Knight / King.


    // Queen / rook / bishop.


}