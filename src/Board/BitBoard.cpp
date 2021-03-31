#include "Bitboard.h"

#include <iostream>

namespace ChessEngine::BitboardUtil {

    /*******************************************************/
    /* General                                             */
    /*******************************************************/

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

}