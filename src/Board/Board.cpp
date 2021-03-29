#include "Board.h"

using namespace ChessEngine::Bitboard_Util;

ChessEngine::Board::Board(const BoardState& state) : state(state) {
    InitOccupancies(Color::White);
    InitOccupancies(Color::Black);
    occupancies[Color::Both] = occupancies[Color::White] | occupancies[Color::Black];
}

void ChessEngine::Board::Draw(){
    for (int rank = 0; rank < 8; rank++) {
        for (int file = 0; file < 8; file++) {
            // Iterate bitboards.
            bool found = false;
            for (int i = 0; i < 12; i++) {
                Color color = (i < 6) ? Color::White : Color::Black;
                auto type = (PieceType)(i % 6);
                if(GetBit(state.pieceBoards[color][type], GetSquareIndex(file, rank))){
                    std::cout << " " << PieceTypeToAscii(type, color) << " ";
                    found = true;
                    break;
                }
            }

            // If we found nothing
            if(!found){
                std::cout << " - ";
            }
        }
        std::cout << std::endl;
    }
}

void ChessEngine::Board::InitOccupancies(Color color){
    Bitboard_Util::Bitboard temp = BITBOARD_EMPTY;
    for (int i = 0; i < 6; i++) {
        temp |= state.pieceBoards[color][i];
    }
    occupancies[color] = temp;
}