#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

#include "BoardState.h"

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
            return occupancies;
        }

    private:

        BoardState state{};

        // Occupancy bitboards for both colors.
        BitboardUtil::Bitboard occupancies[3]{};

        /* Initialize the 2 occupancy bitboards
         * NOTE:Should be called only once at the start.
         * Further updates should be done based on the executed move. */
        void InitOccupancies(Color color);
    };

}

#endif