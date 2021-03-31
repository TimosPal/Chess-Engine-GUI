#include "MoveTables.h"

#include "LeaperPieces.h"
#include "SlidingPieces.h"

namespace ChessEngine::MoveTables {

    // NOTE: performance isn't crucial for these functions
    // since they are run once when producing the tables.

    using namespace ChessEngine::BitboardUtil;
    using namespace ChessEngine::MagicNumbers;
    using namespace ChessEngine::MoveGeneration;

    /*******************************************************/
    /* General                                             */
    /*******************************************************/

    /* Generate a move table based on the given function for each board position. */
    static std::array<Bitboard, 64> InitLeaperMoves(Bitboard GetMoves(Bitboard)) {
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
    static std::array<Bitboard, permutations> InitSlidingMoves() {
        std::array<Bitboard, permutations> slidingMoves = {};

        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {
                uint8_t squareIndex = GetSquareIndex(file, rank);

                SlidingPieces::InitMovesForSquare(slidingMoves, squareIndex, true);
                SlidingPieces::InitMovesForSquare(slidingMoves, squareIndex, false);
            }
        }

        return slidingMoves;
    }

    // Main sliding piece table , contains both rook and bishop attacks.
    static std::array<Bitboard, permutations> slidingMoves = InitSlidingMoves();

    /*******************************************************/
    /* Rook                                                */
    /*******************************************************/

    Bitboard GetRookMoves(uint8_t index, BitboardUtil::Bitboard occupancies) {
        Bitboard blockerMask = SlidingPieces::rookMasks[index];
        DrawBitBoard(blockerMask);
        uint8_t bitCount = SlidingPieces::rookMaskBitCounts[index];
        return slidingMoves[RookMagicHash(occupancies & blockerMask, index, bitCount)];
    }

    /*******************************************************/
    /* Bishop                                              */
    /*******************************************************/

    Bitboard GetBishopMoves(uint8_t index, BitboardUtil::Bitboard occupancies) {
        Bitboard blockerMask = SlidingPieces::bishopMasks[index];
        uint8_t bitCount = SlidingPieces::bishopMaskBitCounts[index];
        return slidingMoves[BishopMagicHash(occupancies & blockerMask, index, bitCount)];
    }

    /*******************************************************/
    /* Queen                                               */
    /*******************************************************/

    Bitboard GetQueenMoves(uint8_t index, BitboardUtil::Bitboard occupancies) {
        return GetBishopMoves(index, occupancies) | GetRookMoves(index, occupancies);
    }

    /*******************************************************/
    /* Pawn                                                */
    /*******************************************************/

    // [team color][square index].
    // NOTE: lambdas are used since InitLeaperMoves expects no color argument. (Only pawns move differ based on color)
    static std::array<std::array<Bitboard, 64>, 2> pawnAttacks =
            {InitLeaperMoves([](auto board) { return LeaperPieces::GetPawnAttacks(board, Color::White); }),
             InitLeaperMoves([](auto board) { return LeaperPieces::GetPawnAttacks(board, Color::Black); })};

    Bitboard GetPawnAttacks(Color color, uint8_t index) {
        return pawnAttacks[color][index];
    }

    /*******************************************************/
    /* Knight                                              */
    /*******************************************************/

    // [square index] only. Black and white have the same attacks.
    static std::array<Bitboard, 64> knightMoves = InitLeaperMoves(LeaperPieces::GetKnightMoves);

    Bitboard GetKnightMoves(uint8_t index) {
        return knightMoves[index];
    }

    /*******************************************************/
    /* King                                                */
    /*******************************************************/

    // [square index] only. Black and white have the same attacks.
    static std::array<Bitboard, 64> kingMoves = InitLeaperMoves(LeaperPieces::GetKingMoves);

    Bitboard GetKingMoves(uint8_t index) {
        return kingMoves[index];
    }

}