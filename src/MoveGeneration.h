#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include <cstdint>

#include "BoardState.h"

namespace ChessEngine::MoveGeneration {

    struct Move{
        uint8_t fromSquareIndex : 3;
        uint8_t toSquareIndex : 3;
    };

    void GenerateMoves(Bitboard_Util::Bitboard board, Color color, PieceType type){
        // iterate over the bitboard , for each isolated bit find the corresponding moves.
        while(board != 0){

            board ^= Bitboard_Util::GetLSB(board);
        }
    }


}

#endif
