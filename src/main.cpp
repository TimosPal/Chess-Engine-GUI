#include <iostream>

#include "Board.h"
#include "Bitboard.h"

using namespace ChessEngine;
using namespace ChessEngine::Bitboard_Util;

int main() {

    for (uint8_t rank = 0; rank < 8; rank++) {
        for (uint8_t file = 0; file < 8; file++) {
            Board(pawnAttacks[0][GetSquareIndex(file, rank)]).Draw();
            getchar();
        }
    }

    return 0;
}
