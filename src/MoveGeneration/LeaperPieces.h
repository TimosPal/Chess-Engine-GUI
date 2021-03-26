#ifndef LEAPERPIECES_H
#define LEAPERPIECES_H

#include "../Board/Bitboard.h"

namespace ChessEngine::LeaperPieces {

    using namespace Bitboard_Util;

    // NOTE: performance isn't crucial for these functions
    // since they are run once when producing the tables.

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
            leftAttack = ShiftUpLeft(board) & not_FileH_Mask;
            rightAttack = ShiftUpRight(board) & not_FileA_Mask;
        }else{
            leftAttack = ShiftDownLeft(board) & not_FileA_Mask;
            rightAttack = ShiftDownRight(board) & not_FileH_Mask;
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
    constexpr Bitboard GetKnightMoves(Bitboard board) {
        Bitboard moves = BITBOARD_EMPTY;

        moves |= (board << 15) & not_FileH_Mask; // Up left.
        moves |= (board << 17) & not_FileA_Mask; // Up right.
        moves |= (board << 6) & not_FileGH_Mask; // Left up.
        moves |= (board << 10) & not_FileAB_Mask; // Right up.

        moves |= (board >> 17) & not_FileH_Mask; // Down left.
        moves |= (board >> 15) & not_FileA_Mask; // Down right.
        moves |= (board >> 10) & not_FileGH_Mask; // Left down.
        moves |= (board >> 6) & not_FileAB_Mask; // Right down.

        return moves;
    }

    // [square index] only. Black and white have the same attacks.
    constexpr std::array<Bitboard, 64> knightMoves = InitMoveTable(GetKnightMoves);

    /*******************************************************/
    /* King                                                */
    /*******************************************************/

    /* Generate the attack moves of all pawns at the given bitboard based on color */
    constexpr Bitboard GetKingMoves(Bitboard board) {
        Bitboard moves = BITBOARD_EMPTY;

        moves |= ShiftUp(board);
        moves |= ShiftDown(board);

        moves |= ShiftLeft(board) & not_FileH_Mask;
        moves |= ShiftUpLeft(board) & not_FileH_Mask;
        moves |= ShiftDownLeft(board) & not_FileA_Mask;

        moves |= ShiftRight(board) & not_FileA_Mask;
        moves |= ShiftUpRight(board) & not_FileA_Mask;
        moves |= ShiftDownRight(board) & not_FileH_Mask;

        return moves;
    }

    // [square index] only. Black and white have the same attacks.
    constexpr std::array<Bitboard, 64> kingMoves = InitMoveTable(GetKingMoves);

}

#endif