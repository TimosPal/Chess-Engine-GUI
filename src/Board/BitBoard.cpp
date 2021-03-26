#include "Bitboard.h"

#include <iostream>

void ChessEngine::Bitboard_Util::DrawBitBoard(Bitboard board){
    for (uint8_t rank = 0; rank < 8; rank++) {
        for (uint8_t file = 0; file < 8; file++) {
            uint8_t square = GetSquareIndex(file, 8 - rank - 1);

            bool hasBit = GetBit(board, square);
            std::cout << (hasBit ? '1' : '-') << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}
