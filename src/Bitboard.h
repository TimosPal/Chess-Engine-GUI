#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>
#include <tuple>

#include "EngineUtil.h"

#define BYTE_MASK 0xffULL
#define BIT_MASK 0x1ULL

#define BITBOARD_EMPTY 0x0ULL

namespace ChessEngine::Bitboard_Util {

    /* We are gonna represent our chess board with bitboards , a 64 bit
     * unsigned integer where each bit corresponds to a position on the board */
    using Bitboard = uint64_t;

    /*******************************************************/
    /* Basic functions                                     */
    /*******************************************************/

    constexpr Bitboard GetBit(const Bitboard& board, uint8_t index){
        return board & (BIT_MASK << index);
    }

    constexpr Bitboard SetBit(const Bitboard& board, uint8_t index){
        return board | (BIT_MASK << index);
    }

    constexpr Bitboard PopBit(const Bitboard& board, uint8_t index){
        return board ^ GetBit(board, index);
    }

    constexpr Bitboard GetLSB(const Bitboard& board){
        return board & -board;
    }

    constexpr uint8_t GetLSBIndex(const Bitboard& board){ // NOTE: maybe use built_in_ffs (?)
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

    constexpr uint8_t GetSquareIndex(uint8_t file, uint8_t rank){
        return rank * 8 + file;
    }

    /* Returns file , rank in a tuple based opn the given square index */
    constexpr std::tuple<uint8_t,uint8_t> GetCoordinates(uint8_t squareIndex){
        return {squareIndex % 8, squareIndex / 8};
    }

    /* Draw the bitboard in a square like form , with 1 and 0s. */
    void DrawBitBoard(Bitboard board);

    /*******************************************************/
    /* Directional shifting                                */
    /*******************************************************/

    /* NOTE: each rank is printed in a Little endian
     * fashion. The origin is located at the bottom left */

    constexpr Bitboard ShiftUp(const Bitboard& board){
        return board << 8ULL;
    }

    constexpr Bitboard ShiftUpLeft(const Bitboard& board){
        return board << 7ULL;
    }

    constexpr Bitboard ShiftUpRight(const Bitboard& board){
        return board << 9ULL;
    }

    constexpr Bitboard ShiftDown(const Bitboard& board){
        return board >> 8ULL;
    }

    constexpr Bitboard ShiftDownLeft(const Bitboard& board){
        return board >> 7ULL;
    }

    constexpr Bitboard ShiftDownRight(const Bitboard& board){
        return board >> 9ULL;
    }

    constexpr Bitboard ShiftLeft(const Bitboard& board){
        return board >> 1ULL;
    }

    constexpr Bitboard ShiftRight(const Bitboard& board){
        return board << 1ULL;
    }

    /*******************************************************/
    /* Basic masks                                         */
    /*******************************************************/

    // TODO: clean up functions.

    constexpr Bitboard GetFileMask(uint8_t file){
        Bitboard board = BITBOARD_EMPTY;

        for (int rank = 0; rank < 8; rank++) {
            board = SetBit(board, GetSquareIndex(file, rank));
        }

        return board;
    }

    constexpr std::array<Bitboard, 8> GenerateFileMasks(){
        std::array<Bitboard, 8> masks = {};

        for (int file = 0; file < 8; file++) {
            masks[file] = GetFileMask(file);
        }

        return masks;
    }

    constexpr Bitboard GetRankMask(uint8_t rank){
        Bitboard board = BITBOARD_EMPTY;

        for (int file = 0; file < 8; file++) {
            board = SetBit(board, GetSquareIndex(file, rank));
        }

        return board;
    }

    constexpr std::array<Bitboard, 8> GenerateRankMasks(){
        std::array<Bitboard, 8> masks = {};

        for (int rank = 0; rank < 8; rank++) {
            masks[rank] = GetRankMask(rank);
        }

        return masks;
    }


    // An array containing masks for each file.
    constexpr std::array<Bitboard, 8> fileMasks = GenerateFileMasks();
    // An array containing masks for each rank.
    constexpr std::array<Bitboard, 8> rankMasks = GenerateRankMasks();

    // Made into variables for easy access.
    constexpr Bitboard notA_Mask = ~fileMasks[File::A];
    constexpr Bitboard notH_Mask = ~fileMasks[File::H];
    constexpr Bitboard r2_Mask = rankMasks[Rank::R2];
    constexpr Bitboard r7_Mask = rankMasks[Rank::R7];

}

#endif