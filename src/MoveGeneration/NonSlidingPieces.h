#ifndef NONSLIDINGPIECES_H
#define NONSLIDINGPIECES_H

#include "../Board/Bitboard.h"

namespace ChessEngine::NonSlidingPieces {

    using namespace Bitboard_Util;

    /*******************************************************/
    /* General                                             */
    /*******************************************************/

    /* Generate a move table based on the given function for each board position. */
    constexpr std::array<Bitboard, 64> InitMoveTable(Bitboard GetMoves(Bitboard)) {
        std::array<Bitboard, 64> movesTable = {};

        for (uint8_t rank = 0; rank < 8; rank++) {
            for (uint8_t file = 0; file < 8; file++) {
                uint8_t squareIndex = GetSquareIndex(file, rank);

                Bitboard board = SetBit(BITBOARD_EMPTY, GetSquareIndex(file, rank));
                movesTable[squareIndex] = GetMoves(board);
            }
        }

        return movesTable;
    }

    /*******************************************************/
    /* Pawn                                                */
    /*******************************************************/

    // When overflowing / underflowing in files A,H we may end up in the
    // opposite direction producing faulty moves. The inverted fileMasks
    // handle those 2 cases.

    // NOTE: having 2 functions per color helps InitMoveTable while also removing
    // the need for a branch inside said functions.

    /* Generate the attack moves of pawns */
    constexpr Bitboard GetPawnAttacks(Bitboard board, Color color) {
        Bitboard leftAttack = BITBOARD_EMPTY, rightAttack = BITBOARD_EMPTY;
        if(color == Color::White) {
            leftAttack = ShiftUpLeft(board) & notH_Mask;
            rightAttack = ShiftUpRight(board) & notA_Mask;
        }else{
            leftAttack = ShiftDownLeft(board) & notA_Mask;
            rightAttack = ShiftDownRight(board) & notH_Mask;
        }
        return leftAttack | rightAttack;
    }

    /* Generate the push moves of pawns */
    constexpr Bitboard GetPawnPushes(Bitboard board, Color color) {
        return (color == Color::White) ? ShiftUp(board) : ShiftDown(board);
    }

    /* Generate the double push moves of pawns , check for occupancy only on the first move. */
    constexpr Bitboard GetDoublePawnPushes(Bitboard board, Bitboard occupancies, Color color) {
        Bitboard pushes = BITBOARD_EMPTY;
        if(color == Color::White) {
            pushes = ShiftUp(board & r2_Mask) & ~occupancies;
            pushes = ShiftUp(pushes);
        }else{
            pushes = ShiftDown(board & r7_Mask) & ~occupancies;
            pushes = ShiftDown(pushes);
        }
        return pushes;
    }

    // [team color][square index].
    // NOTE: lambdas are used since InitMoveTable expects no color argument. (Only pawns move differ based on color)
    constexpr std::array<std::array<Bitboard, 64>, 2> pawnAttacks =
            {InitMoveTable([](auto board) { return GetPawnAttacks(board, Color::White); }),
             InitMoveTable([](auto board) { return GetPawnAttacks(board, Color::Black); })};

    /*******************************************************/
    /* Knight                                              */
    /*******************************************************/

    /* Generate the attack moves of all pawns at the given bitboard based on color */
    constexpr Bitboard GetKnightAttacks(Bitboard board) {
        return 0;
    }

    // [square index] only. Black and white have the same attacks.
    constexpr std::array<Bitboard, 64> knightAttacks = InitMoveTable(GetKnightAttacks);

}

#endif