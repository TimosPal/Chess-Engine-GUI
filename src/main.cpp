#include <iostream>

#include "Board/Board.h"
#include "MoveGeneration/MoveGeneration.h"
#include "MoveGeneration/NonSlidingPieces.h"

using namespace ChessEngine;

int main() {
    BoardState state = {};
    std::string fenPosition = "rnbqkbnr/ppp1p1pp/8/3pPp2/8/8/PPPP1PPP/RNBQKBNR w KQkq f6 1 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);

    MoveGeneration::GenerateMoves(board.GetState(), Color::White, board.GetOccupancies());

    return 0;
}
