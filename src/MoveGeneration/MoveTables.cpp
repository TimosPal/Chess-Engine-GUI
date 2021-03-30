#include "MoveTables.h"

#include "LeaperPieces.h"
#include "SlidingPieces.h"

using namespace ChessEngine;
using namespace ChessEngine::Bitboard_Util;
using namespace ChessEngine::LeaperPieces;
using namespace ChessEngine::SlidingPieces;
using namespace ChessEngine::MoveTables;
using namespace ChessEngine::MagicNumbers;

/*******************************************************/
/* General                                             */
/*******************************************************/

/* Generate a move table based on the given function for each board position. */
static constexpr std::array<Bitboard, 64> InitLeaperMoves(Bitboard GetMoves(Bitboard)) {
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

/* Generate a single move table for rooks and bishops */
constexpr std::array<Bitboard, permutations> InitSlidingMoves(){
    std::array<Bitboard, permutations> slidingMoves = {};

    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            uint8_t squareIndex = GetSquareIndex(file, rank);

            InitMovesForSquare(slidingMoves, squareIndex, true);
            InitMovesForSquare(slidingMoves, squareIndex, false);
        }
    }

    return slidingMoves;
}

// Main sliding piece table , contains both rook and bishop attacks.
static std::array<Bitboard, permutations> slidingMoves = InitSlidingMoves();

/*******************************************************/
/* Rook                                                */
/*******************************************************/

Bitboard ChessEngine::MoveTables::GetPrecalculated_RookAttacks(uint8_t index, Bitboard_Util::Bitboard occupancies){
    Bitboard blockerMask = rookMasks[index];
    uint8_t bitCount = GetBitCount(blockerMask);
    return slidingMoves[RookMagicHash(occupancies, index, bitCount)];
}

/*******************************************************/
/* Bishop                                              */
/*******************************************************/

Bitboard ChessEngine::MoveTables::GetPrecalculated_BishopAttacks(uint8_t index, Bitboard_Util::Bitboard occupancies){
    Bitboard blockerMask = bishopMasks[index];
    uint8_t bitCount = GetBitCount(blockerMask);
    return slidingMoves[BishopMagicHash(occupancies, index, bitCount)];
}

/*******************************************************/
/* Queen                                               */
/*******************************************************/

Bitboard ChessEngine::MoveTables::GetPrecalculated_QueenAttacks(uint8_t index, Bitboard_Util::Bitboard occupancies){
    return GetPrecalculated_BishopAttacks(index, occupancies) || GetPrecalculated_RookAttacks(index, occupancies);
}

/*******************************************************/
/* Pawn                                                */
/*******************************************************/

// [team color][square index].
// NOTE: lambdas are used since InitLeaperMoves expects no color argument. (Only pawns move differ based on color)
static constexpr std::array<std::array<Bitboard, 64>, 2> pawnAttacks =
        {InitLeaperMoves([](auto board) { return LeaperPieces::GetPawnAttacks(board, Color::White); }),
         InitLeaperMoves([](auto board) { return LeaperPieces::GetPawnAttacks(board, Color::Black); })};

Bitboard ChessEngine::MoveTables::GetPrecalculated_PawnAttacks(Color color, uint8_t index){
    return pawnAttacks[color][index];
}

/*******************************************************/
/* Knight                                              */
/*******************************************************/

// [square index] only. Black and white have the same attacks.
static constexpr std::array<Bitboard, 64> knightMoves = InitLeaperMoves(LeaperPieces::GetKnightMoves);

Bitboard ChessEngine::MoveTables::GetPrecalculated_KnightMoves(uint8_t index){
    return knightMoves[index];
}

/*******************************************************/
/* King                                                */
/*******************************************************/

// [square index] only. Black and white have the same attacks.
static constexpr std::array<Bitboard, 64> kingMoves = InitLeaperMoves(LeaperPieces::GetKingMoves);

Bitboard ChessEngine::MoveTables::GetPrecalculated_KingMoves(uint8_t index){
    return kingMoves[index];
}