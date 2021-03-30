#include <iostream>
#include <unistd.h>

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

    MoveGeneration::GeneratePseudoMoves(board.GetState(), Color::White, board.GetOccupancies());
    //Bitboard b = SlidingPieces::rookMasks[GetSquareIndex(File::D, Rank::R4)];
    //Bitboard b = SlidingPieces::GetRookMoves(File::D, Rank::R4, rand());

    return 0;
}
