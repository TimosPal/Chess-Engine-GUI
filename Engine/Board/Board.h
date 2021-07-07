#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

#include "BoardState.h"
#include "BoardOccupancies.h"

#include <iostream>

namespace ChessEngine {

    class Board {
    public:
        explicit Board(const BoardState& state);

        void Draw();

        BoardState& GetState(){
            return state;
        }

        BoardOccupancies& GetOccupancies(){
            return boardOccupancies;
        }

    private:

        BoardState state;
        BoardOccupancies boardOccupancies;
    };

}

#endif