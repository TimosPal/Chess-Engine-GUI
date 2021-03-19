#ifndef BITBOARD_H
#define BITBOARD_H

#include <array>

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

    constexpr Bitboard GetBit(Bitboard board, uint8_t index){
        return board & (BIT_MASK << index);
    }

    constexpr Bitboard SetBit(Bitboard board, uint8_t index){
        return board | (BIT_MASK << index);
    }

    constexpr uint8_t SquareIndex(uint8_t file, uint8_t rank){
        return rank * 8 + file;
    }

    /*******************************************************/
    /* Directional shifting                                */
    /*******************************************************/

    /* NOTE: each rank is printed in a Little endian
     * fashion. The origin is located at the bottom left */

    constexpr Bitboard ShiftUp(Bitboard board){
        return board << 8ULL;
    }

    constexpr Bitboard ShiftUpLeft(Bitboard board){
        return board << 7ULL;
    }

    constexpr Bitboard ShiftUpRight(Bitboard board){
        return board << 9ULL;
    }

    constexpr Bitboard ShiftDown(Bitboard board){
        return board >> 8ULL;
    }

    constexpr Bitboard ShiftDownLeft(Bitboard board){
        return board >> 7ULL;
    }

    constexpr Bitboard ShiftDownRight(Bitboard board){
        return board >> 9ULL;
    }

    constexpr Bitboard ShiftLeft(Bitboard board){
        return board >> 1ULL;
    }

    constexpr Bitboard ShiftRight(Bitboard board){
        return board << 1ULL;
    }


    /*******************************************************/
    /* Pawn attacks                                        */
    /*******************************************************/

    /* Generate the attack moves of a pawn at the given position based on its color */
    constexpr Bitboard GeneratePawnAttacks(uint8_t file, uint8_t rank, Color color){
        Bitboard board = SetBit(BITBOARD_EMPTY, SquareIndex(file, rank));

        if(color == Color::white)
            board |= ShiftUpLeft(board) | ShiftUpRight(board);
        else
            board |= ShiftDownLeft(board) | ShiftDownRight(board);

        return board;
    }

    /* Generate all the attack moves for each board position and color */
    constexpr auto GeneratePawnsAttacks(){
        std::array<std::array<Bitboard, 64>, 2> attacks = {};

        for (uint8_t rank = 0; rank < 8; rank++) {
            for (uint8_t file = 0; file < 8; file++) {
                uint8_t squareIndex = SquareIndex(file, rank);

                attacks[Color::white][squareIndex] = GeneratePawnAttacks(file, rank, Color::white);
                attacks[Color::black][squareIndex] = GeneratePawnAttacks(file, rank, Color::black);
            }
        }

        return attacks;
    }

    // An [team color][square index] array.
    constexpr auto pawnAttacks = GeneratePawnsAttacks();


}

#endif