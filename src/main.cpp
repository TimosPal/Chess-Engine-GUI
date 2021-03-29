#include <iostream>

#include "Board/Board.h"
#include "MoveGeneration/MoveGeneration.h"
#include "MoveGeneration/LeaperPieces.h"
#include "MoveGeneration/SlidingPieces.h"

using namespace ChessEngine;
using namespace ChessEngine::Bitboard_Util;

int main() {
    BoardState state = {};
    std::string fenPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);
    //board.Draw();

    //oveGeneration::GeneratePseudoMoves(board.GetState(), Color::White, board.GetOccupancies());
    DrawBitBoard(SlidingPieces::bishopAttackMasks[GetSquareIndex(3,4)]);

    return 0;
}
