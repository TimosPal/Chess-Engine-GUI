#include "LeaperPieces.h"

namespace ChessEngine::MoveGeneration::LeaperPieces {

    // NOTE: performance isn't crucial for these functions
    // since they are run once when producing the tables.

    using namespace ChessEngine::BitboardUtil;

    /*******************************************************/
    /* Pawn                                                */
    /*******************************************************/

    // When overflowing / underflowing in files A,H we may end up in the
    // opposite direction producing faulty moves. The inverted fileMasks
    // handle those 2 cases.

    /* Generate the attack moves of pawns */
    Bitboard GetPawnAttacks(Bitboard board, Color color) {
        Bitboard leftAttack, rightAttack;
        if (color == Color::White) {
            leftAttack = ShiftUpLeft(board) & not_FileH_Mask;
            rightAttack = ShiftUpRight(board) & not_FileA_Mask;
        } else {
            leftAttack = ShiftDownLeft(board) & not_FileA_Mask;
            rightAttack = ShiftDownRight(board) & not_FileH_Mask;
        }
        return leftAttack | rightAttack;
    }

    /* Generate the push moves of pawns */
    Bitboard GetPawnPushes(Bitboard board, Color color) {
        return (color == Color::White) ? ShiftUp(board) : ShiftDown(board);
    }

    /* Generate the double push moves of pawns , check for occupancy only on the first move. */
    Bitboard GetDoublePawnPushes(Bitboard board, Bitboard occupancies, Color color) {
        Bitboard pushes;
        if (color == Color::White) {
            pushes = ShiftUp(board & r2_Mask) & ~occupancies;
            pushes = ShiftUp(pushes);
        } else {
            pushes = ShiftDown(board & r7_Mask) & ~occupancies;
            pushes = ShiftDown(pushes);
        }
        return pushes;
    }

    /*******************************************************/
    /* Knight                                              */
    /*******************************************************/

    /* Generate the attack moves of all pawns at the given bitboard based on color */
    Bitboard GetKnightMoves(Bitboard board) {
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

    /*******************************************************/
    /* King                                                */
    /*******************************************************/

    /* Generate the attack moves of all pawns at the given bitboard based on color */
    Bitboard GetKingMoves(Bitboard board) {
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

}