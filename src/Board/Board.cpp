#include "Board.h"

using namespace ChessEngine::BitboardUtil;

ChessEngine::Board::Board(const BoardState& state) : state(state) , utilities(state) {}

void ChessEngine::Board::Draw(){
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            auto [type, color] = utilities.squaresOccupants[rank][file];
            std::cout << " " << PieceTypeToString(type, color) << " ";
        }
        std::cout << std::endl;
    }
}