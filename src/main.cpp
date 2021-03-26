#include <iostream>

#include "Board/Board.h"
#include "MoveGeneration/MoveGeneration.h"
#include "MoveGeneration/NonSlidingPieces.h"

using namespace ChessEngine;

int main() {
    BoardState state = {};
    std::string fenPosition = "rnbqkbnr/pppppppp/8/8/8/8/8/R3K2R w KQkq - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);

    MoveGeneration::GeneratePseudoMoves(board.GetState(), Color::White, board.GetOccupancies());
    //Bitboard_Util::DrawBitBoard(Bitboard_Util::kingSideCastling_Mask);

    return 0;
}
