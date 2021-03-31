#include "Bitboard.h"

#include <iostream>

namespace ChessEngine::BitboardUtil {

    /*******************************************************/
    /* Basic functions                                     */
    /*******************************************************/

    Bitboard GetBit(Bitboard board, uint8_t index) {
        return board & (BIT_MASK << index);
    }

    Bitboard SetBit(Bitboard board, uint8_t index) {
        return board | (BIT_MASK << index);
    }

    Bitboard PopBit(Bitboard board, uint8_t index) {
        return board ^ GetBit(board, index);
    }

    Bitboard GetLSB(Bitboard board) {
        return board & -board;
    }

    uint8_t GetLSBIndex(Bitboard board) { // NOTE: maybe use built_in_ffs (?)
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

    uint8_t GetSquareIndex(uint8_t file, uint8_t rank) {
        return rank * 8 + file;
    }

    /* Returns file , rank in a tuple based opn the given square index */
    std::tuple<uint8_t, uint8_t> GetCoordinates(uint8_t squareIndex) {
        return {squareIndex % 8, squareIndex / 8};
    }

    /* Given a board we generate it's index-th permutation. */
    Bitboard GetPermutation(Bitboard board, uint16_t permutationIndex) {
        // permutationIndex is some sort of mask that helps us permutate the board.
        // eg: permutationIndex = 101 , 1st mask bit = 1 , 2nd mask bit = 0 etc.
        // It's like projecting permutationIndex on the board map.
        // This works because by increasing permutationIndex we get all the binary
        // permutations just flattened out.

        Bitboard permutation = BITBOARD_EMPTY;
        int i = 0; // i points to the current i-th bit of the permutationIndex.
        while (board != 0) {
            int lsbIndex = GetLSBIndex(board);

            if (permutationIndex & SetBit(BITBOARD_EMPTY, i)) {
                // project the i-th bit on the board.
                permutation |= SetBit(BITBOARD_EMPTY, lsbIndex);
            }

            board = PopBit(board, lsbIndex);
            i++;
        }

        return permutation;
    }

    uint8_t GetBitCount(Bitboard board) {
        uint8_t count = 0;
        while (board) {
            count++;
            board &= board - 1;
        }
        return count;
    }

    void DrawBitBoard(Bitboard board) {
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

    Bitboard ShiftUp(Bitboard board) {
        return board << 8ULL;
    }

    Bitboard ShiftUpLeft(Bitboard board) {
        return board << 7ULL;
    }

    Bitboard ShiftUpRight(Bitboard board) {
        return board << 9ULL;
    }

    Bitboard ShiftDown(Bitboard board) {
        return board >> 8ULL;
    }

    Bitboard ShiftDownLeft(Bitboard board) {
        return board >> 7ULL;
    }

    Bitboard ShiftDownRight(Bitboard board) {
        return board >> 9ULL;
    }

    Bitboard ShiftLeft(Bitboard board) {
        return board >> 1ULL;
    }

    Bitboard ShiftRight(Bitboard board) {
        return board << 1ULL;
    }

    /*******************************************************/
    /* Basic masks                                         */
    /*******************************************************/

    /* 'From' should be smaller than 'to' */
    static Bitboard GetCastlingMask(uint8_t from, uint8_t to) {
        Bitboard temp = BITBOARD_EMPTY;

        while (from <= to) {
            temp = SetBit(temp, GetSquareIndex(from, Rank::R1));
            temp = SetBit(temp, GetSquareIndex(from, Rank::R8));

            from = (File) (from + 1);
        }

        return temp;
    }

    // File masks.
    const Bitboard fileA_Mask = 0x0101010101010101ULL;
    const Bitboard fileB_Mask = fileA_Mask << 1;
    const Bitboard fileG_Mask = fileA_Mask << 6;
    const Bitboard fileH_Mask = fileA_Mask << 7;

    // Not file masks.
    const Bitboard not_FileA_Mask = ~fileA_Mask;
    const Bitboard not_FileB_Mask = ~fileB_Mask;
    const Bitboard not_FileG_Mask = ~fileG_Mask;
    const Bitboard not_FileH_Mask = ~fileH_Mask;

    const Bitboard not_FileAB_Mask = ~(fileA_Mask | fileB_Mask);
    const Bitboard not_FileGH_Mask = ~(fileH_Mask | fileG_Mask);

    // Rank masks.
    const Bitboard r1_Mask = 0xff;
    const Bitboard r2_Mask = r1_Mask << (8 * 1);
    const Bitboard r7_Mask = r1_Mask << (8 * 6);
    const Bitboard r8_Mask = r1_Mask << (8 * 7);

    // Used in castling.
    const Bitboard kingSideCastling_Mask = GetCastlingMask(File::F, File::G);
    const Bitboard queenSideCastling_Mask = GetCastlingMask(File::B, File::D);

    // Mixed boards , need to mask with appropriate color rank.
    const Bitboard kingsStartingPosBoard =
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::E, Rank::R1)) |
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::E, Rank::R8));

    const Bitboard kingsCastlePosBoard =
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::G, Rank::R1)) |
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::G, Rank::R8));

    const Bitboard queenCastlePosBoard =
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::C, Rank::R1)) |
            SetBit(BITBOARD_EMPTY, GetSquareIndex(File::C, Rank::R8));

}