#include <iostream>

#include "Board.h"
#include "MoveGeneration.h"

using namespace ChessEngine;

int main() {
    BoardState state = {};
    std::string fenPosition = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);
    //board.Draw(Color::White, PieceType::Pawn);

    MoveGeneration::GenerateMoves(Bitboard_Util::SetBit(BITBOARD_EMPTY, Bitboard_Util::GetSquareIndex(File::H, Rank::R6)), White);

    return 0;
}
