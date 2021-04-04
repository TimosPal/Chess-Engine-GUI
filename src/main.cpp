#include <iostream>

#include "FenParser/FenParser.h"
#include "Board/Board.h"
#include "MoveGeneration/PseudoMoves.h"

using namespace ChessEngine;
using namespace ChessEngine::BitboardUtil;

int main() {
    InitEngine(); // Should be called first.

    BoardState state = {};
    std::string fenPosition = "8/8/8/3PpP2/8/8/8/8 w - e6 1 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);
    board.Draw();

    ChessEngine::MoveGeneration::Pseudo::GetAllMoves(board.GetState(), White, board.GetOccupancies());


    return 0;
}
