#include <iostream>

#include "Board/Board.h"
#include "MoveGeneration/MoveGeneration.h"
#include "MoveGeneration/LeaperPieces.h"

using namespace ChessEngine;

int main() {
    BoardState state = {};
    std::string fenPosition = "8/2p1P3/1P6/3N4/1P6/2P1p3/8/8 w - - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);

    MoveGeneration::GeneratePseudoMoves(board.GetState(), Color::White, board.GetOccupancies());

    return 0;
}
