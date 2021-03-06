#include "BoardOccupancies.h"

namespace ChessEngine {

    using namespace BitboardUtil;

    BoardOccupancies::BoardOccupancies(const BoardState& state) {
        InitOccupancies(Color::White, state);
        InitOccupancies(Color::Black, state);
        occupancies[Color::Both] = occupancies[Color::White] | occupancies[Color::Black];

        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file < 8; file++) {
                uint8_t index = GetSquareIndex(file, rank);
                squaresOccupants[index] = state.GetPosType(index);
            }
        }
    }

    void BoardOccupancies::InitOccupancies(Color color, const BoardState& state) {
        BitboardUtil::Bitboard temp = BITBOARD_EMPTY;
        for (int i = 0; i < 6; i++) {
            temp |= state.pieceBoards[color][i];
        }
        occupancies[color] = temp;
    }

}