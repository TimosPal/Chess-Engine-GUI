#include <iostream>

#include "Board/Board.h"
#include "MoveGeneration/MoveGeneration.h"
#include "MoveGeneration/NonSlidingPieces.h"

using namespace ChessEngine;

int main() {
    BoardState state = {};
    std::string fenPosition = "8/8/8/8/2pP4/1p6/PPP5/8 w - - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);

    MoveGeneration::GenerateMoves(board.GetState(), Color::White, board.GetOccupancies());
    //Bitboard_Util::DrawBitBoard(NonSlidingPieces::GetPawnPushes(1, Color::White));

    return 0;
}
