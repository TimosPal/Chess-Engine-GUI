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

    constexpr uint8_t GetSquareIndex(uint8_t file, uint8_t rank){
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
    /* Basic masks                                         */
    /*******************************************************/

    constexpr Bitboard GetFileMask(uint8_t file){
        Bitboard board = BITBOARD_EMPTY;

        for (int rank = 0; rank < 8; rank++) {
            board = SetBit(board, GetSquareIndex(file, rank));
        }

        return board;
    }

    constexpr auto GenerateFileMasks(){
        std::array<Bitboard, 8> masks = {};

        for (int file = 0; file < 8; file++) {
            masks[file] = GetFileMask(file);
        }

        return masks;
    }

    // An array containing masks for each file.
    constexpr auto fileMasks = GenerateFileMasks();
    // Made into variables for easy access.
    constexpr Bitboard notA_Mask = ~fileMasks[File::A];
    constexpr Bitboard notH_Mask = ~fileMasks[File::H];

    /*******************************************************/
    /* Pawn attacks                                        */
    /*******************************************************/

    /* Generate the attack moves of all pawns at the given bitboard based on color */
    constexpr Bitboard GetPawnAttacks(Bitboard board, Color color){
        Bitboard leftAttack = BITBOARD_EMPTY , rightAttack = BITBOARD_EMPTY;
        if(color == Color::white) {
            leftAttack = ShiftUpLeft(board) & notH_Mask;
            rightAttack = ShiftUpRight(board) & notA_Mask;
        }else {
            leftAttack = ShiftDownLeft(board) & notA_Mask;
            rightAttack = ShiftDownRight(board) & notH_Mask;
        }

        // When overflowing / underflowing in files A,H we may end up in the
        // opposite direction producing faulty moves. The inverted fileMasks
        // handle those 2 cases.
        return leftAttack | rightAttack;
    }

    /* Generate the attack moves of a pawn at the given position based on its color */
    constexpr Bitboard GetPawnAttacks(uint8_t file, uint8_t rank, Color color){
        Bitboard board = SetBit(BITBOARD_EMPTY, GetSquareIndex(file, rank));
        return GetPawnAttacks(board, color);
    }

    /* Generate all the attack moves for each board position and color */
    constexpr auto GeneratePawnsAttacks(){
        std::array<std::array<Bitboard, 64>, 2> attacks = {};

        for (uint8_t rank = 0; rank < 8; rank++) {
            for (uint8_t file = 0; file < 8; file++) {
                uint8_t squareIndex = GetSquareIndex(file, rank);

                attacks[Color::white][squareIndex] = GetPawnAttacks(file, rank, Color::white);
                attacks[Color::black][squareIndex] = GetPawnAttacks(file, rank, Color::black);
            }
        }

        return attacks;
    }

    // An [team color][square index] array.
    constexpr auto pawnAttacks = GeneratePawnsAttacks();


}

#endif