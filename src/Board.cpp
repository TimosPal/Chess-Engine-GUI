#include "Board.h"

using namespace ChessEngine::Bitboard_Util;

void ChessEngine::Board::Draw(Color color, PieceType type){
    DrawBitBoard(this->state.pieceBoards[color][type]);
}