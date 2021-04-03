#include "MoveTables.h"

#include "LeaperPieces.h"
#include "SlidingPieces.h"

namespace ChessEngine::MoveGeneration::MoveTables {

    // NOTE: performance isn't crucial for these functions
    // since they are run once when producing the tables.

    using namespace ChessEngine::BitboardUtil;
    using namespace ChessEngine::MagicNumbers;
    using namespace ChessEngine::MoveGeneration;

    /*******************************************************/
    /* General                                             */
    /*******************************************************/

    /* Generate a move table based on the given function for each board position. */
    static std::array<Bitboard, 64> CreateLeaperMoves(Bitboard GetMoves(Bitboard)) {
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
    static std::array<Bitboard, permutations> CreateSlidingMoves() {
        std::array<Bitboard, permutations> moves = {};

        for (int rank = 0; rank < 8; rank++) {
            for (int file = 0; file < 8; file++) {
                uint8_t squareIndex = GetSquareIndex(file, rank);

                SlidingPieces::InitSlidingMoves(moves, squareIndex, true);
                SlidingPieces::InitSlidingMoves(moves, squareIndex, false);
            }
        }

        return moves;
    }

    void InitMoveTables(){
        auto whiteLeaper = [](auto board) { return LeaperPieces::GetPawnAttacks(board, Color::White); };
        auto blackLeaper = [](auto board) { return LeaperPieces::GetPawnAttacks(board, Color::Black); };

        pawnAttacks = {CreateLeaperMoves(whiteLeaper), CreateLeaperMoves(blackLeaper)};
        kingMoves = CreateLeaperMoves(LeaperPieces::GetKingMoves);
        knightMoves = CreateLeaperMoves(LeaperPieces::GetKnightMoves);
        slidingMoves = CreateSlidingMoves();
    }

    // Main sliding piece table , contains both rook and bishop attacks.
    std::array<Bitboard, permutations> slidingMoves{};

    /*******************************************************/
    /* Rook                                                */
    /*******************************************************/

    Bitboard GetRookMoves(uint8_t index, BitboardUtil::Bitboard occupancies) {
        Bitboard blockerMask = SlidingPieces::rookMasks[index];
        return slidingMoves[RookMagicHash(blockerMask & occupancies, index)];
    }

    /*******************************************************/
    /* Bishop                                              */
    /*******************************************************/

    Bitboard GetBishopMoves(uint8_t index, BitboardUtil::Bitboard occupancies) {
        Bitboard blockerMask = SlidingPieces::bishopMasks[index];
        return slidingMoves[BishopMagicHash(blockerMask & occupancies, index)];
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
    // NOTE: lambdas are used since CreateLeaperMoves expects no color argument. (Only pawns move differ based on color)
    std::array<std::array<Bitboard, 64>, 2> pawnAttacks{};

    Bitboard GetPawnAttacks(Color color, uint8_t index) {
        return pawnAttacks[color][index];
    }

    /*******************************************************/
    /* Knight                                              */
    /*******************************************************/

    // [square index] only. Black and white have the same attacks.
    std::array<Bitboard, 64> knightMoves{};

    Bitboard GetKnightMoves(uint8_t index) {
        return knightMoves[index];
    }

    /*******************************************************/
    /* King                                                */
    /*******************************************************/

    // [square index] only. Black and white have the same attacks.
    std::array<Bitboard, 64> kingMoves{};

    Bitboard GetKingMoves(uint8_t index) {
        return kingMoves[index];
    }

}