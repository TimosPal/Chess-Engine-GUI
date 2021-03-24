#include <iostream>

#include "Board.h"
#include "MoveGeneration.h"

using namespace ChessEngine;

int main() {
    BoardState state = {};
    std::string fenPosition = "r1b1kb1r/pp1p1ppp/5P1P/2p5/3P2B1/1q2B3/PPP2P2/RN1QK1NR w kq - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    Board board(state);
    //board.Draw(Color::White, PieceType::Pawn);

    //Bitboard_Util::DrawBitBoard(board.GetOccupancyBitBoard(Color::White));
    MoveGeneration::GenerateAttacks(board.GetPieceBitBoard(Color::White, PieceType::Pawn), White,
                                    board.GetOccupancyBitBoard(Color::Black));

    return 0;
}
