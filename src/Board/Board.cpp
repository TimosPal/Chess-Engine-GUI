#include "Board.h"

using namespace ChessEngine::BitboardUtil;

ChessEngine::Board::Board(const BoardState& state) : state(state) {
    InitOccupancies(Color::White);
    InitOccupancies(Color::Black);
    occupancies[Color::Both] = occupancies[Color::White] | occupancies[Color::Black];
}

void ChessEngine::Board::Draw(){
    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            auto [type, color] = state.GetPosType(GetSquareIndex(file, rank));
            std::cout << " " << PieceTypeToString(type, color) << " ";
        }
        std::cout << std::endl;
    }
}

void ChessEngine::Board::InitOccupancies(Color color){
    BitboardUtil::Bitboard temp = BITBOARD_EMPTY;
    for (int i = 0; i < 6; i++) {
        temp |= state.pieceBoards[color][i];
    }
    occupancies[color] = temp;
}