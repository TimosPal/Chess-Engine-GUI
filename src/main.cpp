#include <iostream>

#include "Board/Board.h"
#include "MoveGeneration/MoveGeneration.h"
#include "MoveGeneration/LeaperPieces.h"

using namespace ChessEngine;

int main() {
    BoardState state = {};
    std::string fenPosition = "8/8/8/4p3/2PK4/2r5/8/8 w - - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);

    MoveGeneration::GeneratePseudoMoves(board.GetState(), Color::White, board.GetOccupancies());

    return 0;
}
