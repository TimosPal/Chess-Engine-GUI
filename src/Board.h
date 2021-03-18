#ifndef BOARD_H
#define BOARD_H

#include <cstdint>
#include "Bitboard.h"

namespace ChessEngine {

    class Board {
    public:

        explicit Board(ChessEngine::Bitboard_Util::Bitboard init) : bitboard(init) {} // NOTE: temp constructor

        void Draw(); // Prints the Board on the console.
    private:
        ChessEngine::Bitboard_Util::Bitboard bitboard;
    };

}

#endif