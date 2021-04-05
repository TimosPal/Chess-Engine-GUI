#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

#include "BoardState.h"
#include "BoardUtilities.h"

#include <iostream>

namespace ChessEngine {

    class Board {
    public:
        explicit Board(const BoardState& state);

        void Draw();

        BoardState GetState(){ //NOTE: For testing only.
            return state;
        }

        BitboardUtil::Bitboard* GetOccupancies(){ //NOTE: For testing only.
            return utilities.occupancies;
        }

    private:

        BoardState state;
        BoardUtilities utilities;

    };

}

#endif