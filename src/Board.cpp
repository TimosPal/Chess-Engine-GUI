#include "Board.h"

#include <iostream>
#include <bitset>

using namespace ChessEngine::Bitboard_Util;

void ChessEngine::Board::Draw(){
    for (uint8_t rank = 0; rank < boardSize; rank++) {
        uint64_t rankValue = GetRank(bitboard, boardSize - rank - 1);
        std::cout << std::bitset<boardSize>(rankValue) << std::endl;
    }
}