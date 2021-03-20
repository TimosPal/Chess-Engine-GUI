#include "Board.h"

#include <iostream>

using namespace ChessEngine::Bitboard_Util;

void ChessEngine::Board::Draw(){ // TODO: put into bitboard.h ?
    for (uint8_t rank = 0; rank < 8; rank++) {
        for (uint8_t file = 0; file < 8; file++) {
            uint8_t square = GetSquareIndex(file, 8 - rank - 1);

            bool hasBit = GetBit(state.pieceBoards[Color::Black][PieceType::Knight], square);
            std::cout << (hasBit ? '1' : '0') << "  ";
        }
        std::cout << std::endl;
    }
}