#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

#include "BoardState.h"
#include "FenParser.h"

namespace ChessEngine {

    class Board {
    public:
        explicit Board(const std::string& fenString) { ParseFenString(fenString, state); }

        void Draw(); // Prints the Board on the console.
    private:

        BoardState state{};
    };

}

#endif