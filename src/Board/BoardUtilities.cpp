#include "BoardUtilities.h"

namespace ChessEngine {

    using namespace BitboardUtil;

    BoardUtilities::BoardUtilities(const BoardState& state) {
        InitOccupancies(Color::White, state);
        InitOccupancies(Color::Black, state);
        occupancies[Color::Both] = occupancies[Color::White] | occupancies[Color::Black];

        for (int rank = 7; rank >= 0; rank--) {
            for (int file = 0; file < 8; file++) {
                squaresOccupants[rank][file] = state.GetPosType(GetSquareIndex(file, rank));
            }
        }
    }

    void BoardUtilities::InitOccupancies(Color color, const BoardState& state) {
        BitboardUtil::Bitboard temp = BITBOARD_EMPTY;
        for (int i = 0; i < 6; i++) {
            temp |= state.pieceBoards[color][i];
        }
        occupancies[color] = temp;
    }

}