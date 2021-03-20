#include <iostream>

#include "Board.h"

using namespace ChessEngine;

int main() {
    Board board("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    board.Draw();

    return 0;
}
