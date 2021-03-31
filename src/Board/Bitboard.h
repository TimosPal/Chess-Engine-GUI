#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>
#include <tuple>

#include "../Utilities/EngineUtil.h"

#define BIT_MASK 0x1ULL

#define BITBOARD_EMPTY 0x0ULL

namespace ChessEngine::BitboardUtil {

    /* We are gonna represent our chess board with bitboards , a 64 bit
     * unsigned integer where each bit corresponds to a position on the board */
    using Bitboard = uint64_t;

    /*******************************************************/
    /* Basic functions                                     */
    /*******************************************************/

    Bitboard GetBit(Bitboard board, uint8_t index);
    Bitboard SetBit(Bitboard board, uint8_t index);
    Bitboard PopBit(Bitboard board, uint8_t index);
    Bitboard GetLSB(Bitboard board);

    uint8_t GetBitCount(Bitboard board);
    uint8_t GetLSBIndex(Bitboard board);
    uint8_t GetSquareIndex(uint8_t file, uint8_t rank);

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

    // File masks.
    extern Bitboard fileA_Mask;
    extern Bitboard fileB_Mask;
    extern Bitboard fileG_Mask;
    extern Bitboard fileH_Mask;

    // Not file masks.
    extern Bitboard not_FileA_Mask;
    extern Bitboard not_FileB_Mask;
    extern Bitboard not_FileG_Mask;
    extern Bitboard not_FileH_Mask;

    extern Bitboard not_FileAB_Mask;
    extern Bitboard not_FileGH_Mask;

    // Rank masks.
    extern Bitboard r1_Mask;
    extern Bitboard r2_Mask;
    extern Bitboard r7_Mask;
    extern Bitboard r8_Mask;

    // Used in castling.
    extern Bitboard kingSideCastling_Mask;
    extern Bitboard queenSideCastling_Mask;

    // Mixed boards , need to mask with appropriate color rank.
    extern Bitboard kingsStartingPosBoard;
    extern Bitboard kingsCastlePosBoard;
    extern Bitboard queenCastlePosBoard;

}

#endif