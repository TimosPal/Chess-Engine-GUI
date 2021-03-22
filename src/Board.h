#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

#include "BoardState.h"
#include "FenParser.h"

#include <iostream>

namespace ChessEngine {

    class Board {
    public:
        explicit Board(const BoardState& state) : state(state) {
            UpdateOccupancies(Color::White);
            UpdateOccupancies(Color::Black);
        }

        void Draw(Color color, PieceType type);

        Bitboard_Util::Bitboard GetPieceBitBoard(Color color, PieceType type){ //NOTE: For testing only.
            return state.pieceBoards[color][type];
        }

        Bitboard_Util::Bitboard GetOccupancyBitBoard(Color color){ //NOTE: For testing only.
            return occupancies[color];
        }

    private:

        BoardState state{};

        // Occupancy bitboards for both colors.
        Bitboard_Util::Bitboard occupancies[2]{};

        void UpdateOccupancies(Color color){
            Bitboard_Util::Bitboard temp = BITBOARD_EMPTY;
            for (int i = 0; i < 6; i++) {
                temp |= state.pieceBoards[color][i];
            }
            occupancies[color] = temp;
        }
    };

}

#endif