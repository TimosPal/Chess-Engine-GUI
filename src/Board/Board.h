#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

#include "BoardState.h"
#include "../FenParser/FenParser.h"

#include <iostream>

namespace ChessEngine {

    class Board {
    public:
        explicit Board(const BoardState& state) : state(state) {
            InitOccupancies(Color::White);
            InitOccupancies(Color::Black);
            occupancies[Color::Both] = occupancies[Color::White] | occupancies[Color::Black];
        }

        void Draw(Color color, PieceType type);

        BoardState GetState(){ //NOTE: For testing only.
            return state;
        }

        Bitboard_Util::Bitboard* GetOccupancies(){ //NOTE: For testing only.
            return occupancies;
        }

    private:

        BoardState state{};

        // Occupancy bitboards for both colors.
        Bitboard_Util::Bitboard occupancies[3]{};

        /* Initialize the 2 occupancy bitboards
         * NOTE:Should be called only once at the start.
         * Further updates should be done based on the executed move. */
        void InitOccupancies(Color color){
            Bitboard_Util::Bitboard temp = BITBOARD_EMPTY;
            for (int i = 0; i < 6; i++) {
                temp |= state.pieceBoards[color][i];
            }
            occupancies[color] = temp;
        }
    };

}

#endif