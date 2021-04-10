#include <iostream>

#include <FenParser/FenParser.h>

#include <Board/Board.h>

#include <MoveGeneration/PseudoMoves.h>
#include <MoveGeneration/MoveGeneration.h>

using namespace ChessEngine;
using namespace ChessEngine::BitboardUtil;

int main() {
    InitEngine(); // Should be called first.

    BoardState state = {};
    std::string fenPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);
    board.Draw();

    auto moves = MoveGeneration::Pseudo::GetAllMoves(board.GetState(), White, board.GetUtilities());
    MoveGeneration::PrintMoves(moves);

    return 0;
}
