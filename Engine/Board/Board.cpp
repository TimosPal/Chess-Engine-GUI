#include "Board.h"

using namespace ChessEngine::BitboardUtil;

ChessEngine::Board::Board(const BoardState& state) : state(state) , utilities(state) {}

void ChessEngine::Board::Draw(){
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            uint8_t index = GetSquareIndex(file, rank);
            auto [type, color] = utilities.squaresOccupants[index];
            std::cout << " " << PieceTypeToChar(type, color) << " ";
        }
        std::cout << std::endl;
    }
}