#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

#include "BoardState.h"
#include "FenParser.h"

#include <iostream>

namespace ChessEngine {

    class Board {
    public:
        explicit Board(const BoardState& state) : state(state) {}

        void Draw(Color color, PieceType type);
    private:

        BoardState state{};
    };

}

#endif