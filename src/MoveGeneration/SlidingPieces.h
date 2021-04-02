#ifndef SLIDING_PIECES_H
#define SLIDING_PIECES_H

#include "../Board/Bitboard.h"
#include "MagicNumbers.h"

namespace ChessEngine::MoveGeneration::SlidingPieces {

    /*******************************************************/
    /* General                                             */
    /*******************************************************/

    void InitBlockerMasks();

    /* Generate a move table based on the given function for each board position. */
    std::array<BitboardUtil::Bitboard, 64> CreateMasksTable(BitboardUtil::Bitboard getMask(uint8_t, uint8_t));

    /* Generate a mask based on an initial position and direction */
    BitboardUtil::Bitboard GetDirectionalBlockerMask(uint8_t file, uint8_t rank, int8_t dirX, int8_t dirY, bool checkX, bool checkY);

    /* Generate an attack mask based on an initial position , direction and occupancies */
    BitboardUtil::Bitboard GetDirectionalMoves(uint8_t file, uint8_t rank, int8_t dirX, int8_t dirY, BitboardUtil::Bitboard occupancies);


    /*******************************************************/
    /* Rook                                                */
    /*******************************************************/

    BitboardUtil::Bitboard GetRookMask(uint8_t file, uint8_t rank);
    BitboardUtil::Bitboard GetRookMoves(uint8_t file, uint8_t rank, BitboardUtil::Bitboard occupancies);

    /*******************************************************/
    /* Bishop                                              */
    /*******************************************************/

    BitboardUtil::Bitboard GetBishopMask(uint8_t file, uint8_t rank);
    BitboardUtil::Bitboard GetBishopMoves(uint8_t file, uint8_t rank, BitboardUtil::Bitboard occupancies);

    void InitSlidingMoves(std::array<BitboardUtil::Bitboard, MagicNumbers::permutations>& slidingMoves, uint8_t squareIndex, bool forBishop);

    /*******************************************************/
    /* Attack masks                                        */
    /*******************************************************/

    // Blocker masks , to be used in magic bitboards.
    extern std::array<BitboardUtil::Bitboard, 64> rookMasks;
    extern std::array<BitboardUtil::Bitboard, 64> bishopMasks;

}

#endif