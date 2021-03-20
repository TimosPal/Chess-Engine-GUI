#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include "Bitboard.h"

namespace ChessEngine {

    class Board {
    public:

        void Draw(); // Prints the Board on the console.
    private:
        // A board with 6 bitboards for each color , 1 bitboard per unique piece type.
        // Use Color / PieceType for indexing
        Bitboard_Util::Bitboard pieceBoards[2][6];
    };

}

#endif