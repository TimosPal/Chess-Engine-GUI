#include "Bitboard.h"

#include <iostream>

using namespace ChessEngine;

/*******************************************************/
/* Basic functions                                     */
/*******************************************************/

BitboardUtil::Bitboard BitboardUtil::GetBit(Bitboard board, uint8_t index){
    return board & (BIT_MASK << index);
}

BitboardUtil::Bitboard BitboardUtil::SetBit(Bitboard board, uint8_t index){
    return board | (BIT_MASK << index);
}

BitboardUtil::Bitboard BitboardUtil::PopBit(Bitboard board, uint8_t index){
    return board ^ GetBit(board, index);
}

BitboardUtil::Bitboard BitboardUtil::GetLSB(Bitboard board){
    return board & -board;
}

uint8_t BitboardUtil::GetLSBIndex(Bitboard board){ // NOTE: maybe use built_in_ffs (?)
    /* De Bruijn bitscan algorithm */
    const uint64_t debruijn64 = 0x03f79d71b4cb0a89;
    const uint8_t index64[64] = {
            0,  1, 48,  2, 57, 49, 28,  3,
            61, 58, 50, 42, 38, 29, 17,  4,
            62, 55, 59, 36, 53, 51, 43, 22,
            45, 39, 33, 30, 24, 18, 12,  5,
            63, 47, 56, 27, 60, 41, 37, 16,
            54, 35, 52, 21, 44, 32, 23, 11,
            46, 26, 40, 15, 34, 20, 31, 10,
            25, 14, 19,  9, 13,  8,  7,  6
    };

    return index64[(GetLSB(board) * debruijn64) >> 58];
}

uint8_t BitboardUtil::GetSquareIndex(uint8_t file, uint8_t rank){
    return rank * 8 + file;
}

/* Returns file , rank in a tuple based opn the given square index */
std::tuple<uint8_t, uint8_t> BitboardUtil::GetCoordinates(uint8_t squareIndex){
    return {squareIndex % 8, squareIndex / 8};
}

/* Given a board we generate it's index-th permutation. */
BitboardUtil::Bitboard BitboardUtil::GetPermutation(Bitboard board, uint16_t permutationIndex){
    // permutationIndex is some sort of mask that helps us permutate the board.
    // eg: permutationIndex = 101 , 1st mask bit = 1 , 2nd mask bit = 0 etc.
    // It's like projecting permutationIndex on the board map.
    // This works because by increasing permutationIndex we get all the binary
    // permutations just flattened out.

    Bitboard permutation = BITBOARD_EMPTY;
    int i = 0; // i points to the current i-th bit of the permutationIndex.
    while(board != 0){
        int lsbIndex = GetLSBIndex(board);

        if(permutationIndex & SetBit(BITBOARD_EMPTY, i)){
            // project the i-th bit on the board.
            permutation |= SetBit(BITBOARD_EMPTY, lsbIndex);
        }

        board = PopBit(board, lsbIndex);
        i++;
    }

    return permutation;
}

uint8_t BitboardUtil::GetBitCount(Bitboard board) {
    uint8_t count = 0;
    while (board) {
        count++;
        board &= board - 1;
    }
    return count;
}

void BitboardUtil::DrawBitBoard(Bitboard board){
    for (uint8_t rank = 0; rank < 8; rank++) {
        for (uint8_t file = 0; file < 8; file++) {
            uint8_t square = GetSquareIndex(file, 8 - rank - 1);

            bool hasBit = GetBit(board, square);
            std::cout << (hasBit ? '1' : '-') << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}


/*******************************************************/
/* Directional shifting                                */
/*******************************************************/

/* NOTE: each rank is printed in a Little endian
 * fashion. The origin is located at the bottom left */

BitboardUtil::Bitboard ShiftUp(BitboardUtil::Bitboard board){
    return board << 8ULL;
}

BitboardUtil::Bitboard ShiftUpLeft(BitboardUtil::Bitboard board){
    return board << 7ULL;
}

BitboardUtil::Bitboard ShiftUpRight(BitboardUtil::Bitboard board){
    return board << 9ULL;
}

BitboardUtil::Bitboard ShiftDown(BitboardUtil::Bitboard board){
    return board >> 8ULL;
}

BitboardUtil::Bitboard ShiftDownLeft(BitboardUtil::Bitboard board){
    return board >> 7ULL;
}

BitboardUtil::Bitboard ShiftDownRight(BitboardUtil::Bitboard board){
    return board >> 9ULL;
}

BitboardUtil::Bitboard ShiftLeft(BitboardUtil::Bitboard board){
    return board >> 1ULL;
}

BitboardUtil::Bitboard ShiftRight(BitboardUtil::Bitboard board){
    return board << 1ULL;
}

/*******************************************************/
/* Basic masks                                         */
/*******************************************************/

/* 'From' should be smaller than 'to' */
static BitboardUtil::Bitboard GetCastlingMask(uint8_t from, uint8_t to){
    BitboardUtil::Bitboard temp = BITBOARD_EMPTY;

    while(from <= to){
        temp = BitboardUtil::SetBit(temp, BitboardUtil::GetSquareIndex(from, Rank::R1));
        temp = BitboardUtil::SetBit(temp, BitboardUtil::GetSquareIndex(from, Rank::R8));

        from = (File)(from + 1);
    }

    return temp;
}

// File masks.
BitboardUtil::Bitboard BitboardUtil::fileA_Mask = 0x0101010101010101ULL;
BitboardUtil::Bitboard BitboardUtil::fileB_Mask = fileA_Mask << 1;
BitboardUtil::Bitboard BitboardUtil::fileG_Mask = fileA_Mask << 6;
BitboardUtil::Bitboard BitboardUtil::fileH_Mask = fileA_Mask << 7;

// Not file masks.
BitboardUtil::Bitboard BitboardUtil::not_FileA_Mask = ~fileA_Mask;
BitboardUtil::Bitboard BitboardUtil::not_FileB_Mask = ~fileB_Mask;
BitboardUtil::Bitboard BitboardUtil::not_FileG_Mask = ~fileG_Mask;
BitboardUtil::Bitboard BitboardUtil::not_FileH_Mask = ~fileH_Mask;

BitboardUtil::Bitboard BitboardUtil::not_FileAB_Mask = ~(fileA_Mask | fileB_Mask);
BitboardUtil::Bitboard BitboardUtil::not_FileGH_Mask = ~(fileH_Mask | fileG_Mask);

// Rank masks.
BitboardUtil::Bitboard BitboardUtil::r1_Mask = 0xff;
BitboardUtil::Bitboard BitboardUtil::r2_Mask = r1_Mask << (8 * 1);
BitboardUtil::Bitboard BitboardUtil::r7_Mask = r1_Mask << (8 * 6);
BitboardUtil::Bitboard BitboardUtil::r8_Mask = r1_Mask << (8 * 7);

// Used in castling.
BitboardUtil::Bitboard BitboardUtil::kingSideCastling_Mask = GetCastlingMask(File::F, File::G);
BitboardUtil::Bitboard BitboardUtil::queenSideCastling_Mask = GetCastlingMask(File::B, File::D);

// Mixed boards , need to mask with appropriate color rank.
BitboardUtil::Bitboard kingsStartingPosBoard = BitboardUtil::SetBit(BITBOARD_EMPTY, BitboardUtil::GetSquareIndex(File::E, Rank::R1)) |
                                               BitboardUtil::SetBit(BITBOARD_EMPTY, BitboardUtil::GetSquareIndex(File::E, Rank::R8));
BitboardUtil::Bitboard kingsCastlePosBoard = BitboardUtil::SetBit(BITBOARD_EMPTY, BitboardUtil::GetSquareIndex(File::G, Rank::R1)) |
                                             BitboardUtil::SetBit(BITBOARD_EMPTY, BitboardUtil::GetSquareIndex(File::G, Rank::R8));
BitboardUtil::Bitboard queenCastlePosBoard = BitboardUtil::SetBit(BITBOARD_EMPTY, BitboardUtil::GetSquareIndex(File::C, Rank::R1)) |
                                             BitboardUtil::SetBit(BITBOARD_EMPTY, BitboardUtil::GetSquareIndex(File::C, Rank::R8));
