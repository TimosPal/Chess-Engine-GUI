#ifndef ATTACKS_H
#define ATTACKS_H

#include "Bitboard.h"

namespace ChessEngine::Attacks {

    using namespace Bitboard_Util;

    /*******************************************************/
    /* Pawn attacks                                        */
    /*******************************************************/

    /* Generate the attack moves of all pawns at the given bitboard based on color */
    constexpr Bitboard GetPawnAttacks(Bitboard board, Color color) {
        Bitboard leftAttack = BITBOARD_EMPTY, rightAttack = BITBOARD_EMPTY;
        if (color == Color::White) {
            leftAttack = ShiftUpLeft(board) & notH_Mask;
            rightAttack = ShiftUpRight(board) & notA_Mask;
        } else {
            leftAttack = ShiftDownLeft(board) & notA_Mask;
            rightAttack = ShiftDownRight(board) & notH_Mask;
        }

        // When overflowing / underflowing in files A,H we may end up in the
        // opposite direction producing faulty moves. The inverted fileMasks
        // handle those 2 cases.
        return leftAttack | rightAttack;
    }

    /* Generate the attack moves of a pawn at the given position based on its color */
    constexpr Bitboard GetPawnAttacks(uint8_t file, uint8_t rank, Color color) {
        Bitboard board = SetBit(BITBOARD_EMPTY, GetSquareIndex(file, rank));
        return GetPawnAttacks(board, color);
    }

    /* Generate all the attack moves for each board position and color */
    constexpr std::array<std::array<Bitboard, 64>, 2> GeneratePawnsAttacks() {
        std::array<std::array<Bitboard, 64>, 2> attacks = {};

        for (uint8_t rank = 0; rank < 8; rank++) {
            for (uint8_t file = 0; file < 8; file++) {
                uint8_t squareIndex = GetSquareIndex(file, rank);

                attacks[Color::White][squareIndex] = GetPawnAttacks(file, rank, Color::White);
                attacks[Color::Black][squareIndex] = GetPawnAttacks(file, rank, Color::Black);
            }
        }

        return attacks;
    }

    // An [team color][square index] array.
    constexpr std::array<std::array<Bitboard, 64>, 2> pawnAttacks = GeneratePawnsAttacks();

}

#endif