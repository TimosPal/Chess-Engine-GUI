#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>
#include <tuple>

#include "../Utilities/Utilities.h"

#define BIT_MASK 0x1ULL
#define BITBOARD_EMPTY 0x0ULL

namespace ChessEngine::BitboardUtil {

    /* We are gonna represent our chess board with bitboards , a 64 bit
     * unsigned integer where each bit corresponds to a position on the board */
    using Bitboard = uint64_t;

    /*******************************************************/
    /* Basic constexpr functions                           */
    /*******************************************************/

    constexpr Bitboard GetBit(Bitboard board, uint8_t index) {
        return board & (BIT_MASK << index);
    }

    constexpr Bitboard SetBit(Bitboard board, uint8_t index) {
        return board | (BIT_MASK << index);
    }

    constexpr Bitboard PopBit(Bitboard board, uint8_t index) {
        return board ^ GetBit(board, index);
    }

    constexpr Bitboard GetLSB(Bitboard board) {
        return board & -board;
    }

    constexpr uint8_t GetBitCount(Bitboard board) {
        uint8_t count = 0;
        while (board) {
            count++;
            board &= board - 1;
        }
        return count;
    }

    constexpr uint8_t GetLSBIndex(Bitboard board) { // NOTE: maybe use built_in_ffs (?)
        /* De Bruijn bitscan algorithm */
        const uint64_t debruijn64 = 0x03f79d71b4cb0a89;
        const uint8_t index64[64] = {
                0, 1, 48, 2, 57, 49, 28, 3,
                61, 58, 50, 42, 38, 29, 17, 4,
                62, 55, 59, 36, 53, 51, 43, 22,
                45, 39, 33, 30, 24, 18, 12, 5,
                63, 47, 56, 27, 60, 41, 37, 16,
                54, 35, 52, 21, 44, 32, 23, 11,
                46, 26, 40, 15, 34, 20, 31, 10,
                25, 14, 19, 9, 13, 8, 7, 6
        };

        return index64[(GetLSB(board) * debruijn64) >> 58];
    }

    constexpr uint8_t GetSquareIndex(int file, int rank) {
        return rank * 8 + file;
    }

    constexpr Bitboard SwapBit(Bitboard board, uint8_t from, uint8_t to){ // TODO: faster approach?
        return SetBit(PopBit(board, from), to);
    }

    /*******************************************************/
    /* General                                             */
    /*******************************************************/

    /* Returns file , rank in a tuple based opn the given square index */
    std::tuple<uint8_t, uint8_t> GetCoordinates(uint8_t squareIndex);

    /* Given a board we generate it's index-th permutation. */
    Bitboard GetPermutation(Bitboard board, uint16_t permutationIndex);

    /* Draw the bitboard in a square like form , with '1' and '-'. */
    /* NOTE: each rank is printed in a Little endian
     * fashion. The origin is located at the bottom left */
    void DrawBitBoard(Bitboard board);

    /*******************************************************/
    /* Directional shifting                                */
    /*******************************************************/

    Bitboard ShiftUp(Bitboard board);
    Bitboard ShiftUpLeft(Bitboard board);
    Bitboard ShiftUpRight(Bitboard board);
    Bitboard ShiftDown(Bitboard board);
    Bitboard ShiftDownLeft(Bitboard board);
    Bitboard ShiftDownRight(Bitboard board);
    Bitboard ShiftLeft(Bitboard board);
    Bitboard ShiftRight(Bitboard board);

    /*******************************************************/
    /* Basic masks                                         */
    /*******************************************************/

    /* 'From' should be smaller than 'to' */
    constexpr Bitboard GetCastlingMask(uint8_t from, uint8_t to) {
        Bitboard temp = BITBOARD_EMPTY;

        while (from <= to) {
            temp = SetBit(temp, GetSquareIndex(from, Rank::R1));
            temp = SetBit(temp, GetSquareIndex(from, Rank::R8));

            from++;
        }

        return temp;
    }

    // File masks.
    constexpr Bitboard fileA_Mask = 0x0101010101010101ULL;
    constexpr Bitboard fileB_Mask = fileA_Mask << 1;
    constexpr Bitboard fileG_Mask = fileA_Mask << 6;
    constexpr Bitboard fileH_Mask = fileA_Mask << 7;

    // Not file masks.
    constexpr Bitboard not_FileA_Mask = ~fileA_Mask;
    constexpr Bitboard not_FileB_Mask = ~fileB_Mask;
    constexpr Bitboard not_FileG_Mask = ~fileG_Mask;
    constexpr Bitboard not_FileH_Mask = ~fileH_Mask;

    constexpr Bitboard not_FileAB_Mask = ~(fileA_Mask | fileB_Mask);
    constexpr Bitboard not_FileGH_Mask = ~(fileH_Mask | fileG_Mask);

    // Rank masks.
    constexpr Bitboard r1_Mask = 0xff;
    constexpr Bitboard r2_Mask = r1_Mask << (8 * 1);
    constexpr Bitboard r3_Mask = r1_Mask << (8 * 2);
    constexpr Bitboard r6_Mask = r1_Mask << (8 * 5);
    constexpr Bitboard r7_Mask = r1_Mask << (8 * 6);
    constexpr Bitboard r8_Mask = r1_Mask << (8 * 7);

    // Used in castling.
    constexpr Bitboard kingSideCastling_Mask = GetCastlingMask(File::F, File::G);
    constexpr Bitboard queenSideCastling_Mask = GetCastlingMask(File::B, File::D);

    // Mixed boards , need to mask with appropriate color rank.
    constexpr Bitboard kingsStartingPosBoard =
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::E, Rank::R1)) |
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::E, Rank::R8));

    constexpr Bitboard kingsCastlePosBoard =
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::G, Rank::R1)) |
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::G, Rank::R8));

    constexpr Bitboard queenCastlePosBoard =
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::C, Rank::R1)) |
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::C, Rank::R8));

    constexpr Bitboard queenRooksBoard =
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::A, Rank::R1)) |
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::A, Rank::R8));

    constexpr Bitboard kingRooksBoard =
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::H, Rank::R1)) |
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::H, Rank::R8));


}

#endif