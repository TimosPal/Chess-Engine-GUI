#include "Board.h"
#include "Bitboard.h"

using namespace ChessEngine;
using namespace ChessEngine::Bitboard_Util;

int main() {
    Bitboard bb = BITBOARD_EMPTY;
    bb = SetBit(bb, SquareIndex(0,0));
    bb = SetBit(bb, SquareIndex(1,0));
    bb = SetBit(bb, SquareIndex(2,0));


    Board board(ShiftUp(bb));
    board.Draw();

    return 0;
}
