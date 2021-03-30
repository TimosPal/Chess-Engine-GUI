#include "MoveTables.h"

#include "LeaperPieces.h"

using namespace ChessEngine;
using namespace ChessEngine::Bitboard_Util;
using namespace ChessEngine::LeaperPieces;
using namespace ChessEngine::MoveTables;

/*******************************************************/
/* General                                             */
/*******************************************************/

/* Generate a move table based on the given function for each board position. */
static constexpr std::array<Bitboard, 64> InitMoveTable(Bitboard GetMoves(Bitboard)) {
    std::array<Bitboard, 64> movesTable = {};

    for (uint8_t rank = 0; rank < 8; rank++) {
        for (uint8_t file = 0; file < 8; file++) {
            uint8_t squareIndex = GetSquareIndex(file, rank);

            Bitboard board = SetBit(BITBOARD_EMPTY, GetSquareIndex(file, rank));
            movesTable[squareIndex] = GetMoves(board);
        }
    }

    return movesTable;
}

/*******************************************************/
/* Pawn                                                */
/*******************************************************/

// [team color][square index].
// NOTE: lambdas are used since InitMoveTable expects no color argument. (Only pawns move differ based on color)
static constexpr std::array<std::array<Bitboard, 64>, 2> pawnAttacks =
        {InitMoveTable([](auto board) { return LeaperPieces::GetPawnAttacks(board, Color::White); }),
         InitMoveTable([](auto board) { return LeaperPieces::GetPawnAttacks(board, Color::Black); })};

Bitboard ChessEngine::MoveTables::GetPrecalculated_PawnAttacks(Color color, uint8_t index){
    return pawnAttacks[color][index];
}

/*******************************************************/
/* Knight                                              */
/*******************************************************/

// [square index] only. Black and white have the same attacks.
static constexpr std::array<Bitboard, 64> knightMoves = InitMoveTable(LeaperPieces::GetKnightMoves);

Bitboard ChessEngine::MoveTables::GetPrecalculated_KnightMoves(uint8_t index){
    return knightMoves[index];
}

/*******************************************************/
/* King                                                */
/*******************************************************/

// [square index] only. Black and white have the same attacks.
static constexpr std::array<Bitboard, 64> kingMoves = InitMoveTable(LeaperPieces::GetKingMoves);

Bitboard ChessEngine::MoveTables::GetPrecalculated_KingMoves(uint8_t index){
    return kingMoves[index];
}