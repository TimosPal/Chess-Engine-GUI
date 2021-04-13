#ifndef BOARD_UTILITIES_H
#define BOARD_UTILITIES_H

#include "BoardState.h"

namespace ChessEngine {

    struct BoardUtilities {
        // Occupancy bitboards for both colors.
        BitboardUtil::Bitboard occupancies[3]{};
        std::tuple<PieceType, Color> squaresOccupants[64];

        explicit BoardUtilities(const BoardState& state);

        /* Initialize the 2 occupancy bitboards
        * NOTE:Should be called only once at the start.
        * Further updates should be done based on the executed move. */
        void InitOccupancies(Color color, const BoardState& state);

    };

}

#endif
