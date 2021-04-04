#include <iostream>

#include "FenParser/FenParser.h"
#include "Board/Board.h"
#include "MoveGeneration/PseudoMoves.h"

using namespace ChessEngine;
using namespace ChessEngine::BitboardUtil;

int main() {
    InitEngine(); // Should be called first.

    BoardState state = {};
    std::string fenPosition = "8/8/2p5/N7/4B3/1p6/8/8 w - - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);
    board.Draw();

    ChessEngine::MoveGeneration::Pseudo::GetAllMoves(board.GetState(), White, board.GetOccupancies());


    return 0;
}
