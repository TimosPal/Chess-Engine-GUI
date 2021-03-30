#include <iostream>
#include <unistd.h>

#include "FenParser/FenParser.h"
#include "Board/Board.h"
#include "MoveGeneration/MoveGeneration.h"
#include "MoveGeneration/MoveTables.h"

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

    Bitboard a = MoveTables::GetQueenMoves(GetSquareIndex(File::E, Rank::R3) , board.GetOccupancies()[2]);
    DrawBitBoard(a);


    return 0;
}
