#ifndef SLIDINGPIECES_H
#define SLIDINGPIECES_H

#include "../Board/Bitboard.h"
#include "MagicNumbers.h"

namespace ChessEngine::MoveGeneration::SlidingPieces {

    /*******************************************************/
    /* General                                             */
    /*******************************************************/

    /* Generate a move table based on the given function for each board position. */
    std::array<BitboardUtil::Bitboard, 64> InitMasksTable(BitboardUtil::Bitboard getMask(uint8_t, uint8_t));

    /* Generate a mask based on an initial position and direction */
    BitboardUtil::Bitboard GetDirectionalBlockerMask(uint8_t file, uint8_t rank, int8_t dirX, int8_t dirY);

    /* Generate an attack mask based on an initial position , direction and occupancies */
    BitboardUtil::Bitboard GetDirectionalMoves(uint8_t file, uint8_t rank, int8_t dirX, int8_t dirY, BitboardUtil::Bitboard occupancies);

    /*******************************************************/
    /* Rook                                                */
    /*******************************************************/

    BitboardUtil::Bitboard GetRookBlockerMask(uint8_t file, uint8_t rank);

    BitboardUtil::Bitboard GetRookMoves(uint8_t file, uint8_t rank, BitboardUtil::Bitboard occupancies);

    /*******************************************************/
    /* Bishop                                              */
    /*******************************************************/

    BitboardUtil::Bitboard GetBishopBlockerMask(uint8_t file, uint8_t rank);

    BitboardUtil::Bitboard GetBishopMoves(uint8_t file, uint8_t rank, BitboardUtil::Bitboard occupancies);

    void InitMovesForSquare(std::array<BitboardUtil::Bitboard, MagicNumbers::permutations>& slidingMoves, uint8_t squareIndex, bool forBishop);

    /*******************************************************/
    /* Attack masks                                        */
    /*******************************************************/

    // Blocker masks , to be used in magic bitboards.
    extern const std::array<BitboardUtil::Bitboard, 64> rookMasks;
    extern const std::array<BitboardUtil::Bitboard, 64> bishopMasks;

    // Used instead of GetBitCount in run time calculations.
    extern const std::array<uint64_t , 64> rookMaskBitCounts;
    extern const std::array<uint64_t , 64> bishopMaskBitCounts;


}

#endif