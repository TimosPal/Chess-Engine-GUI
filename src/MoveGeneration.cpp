#include "MoveGeneration.h"

#include "Attacks.h"

using namespace ChessEngine::Bitboard_Util;

void ChessEngine::MoveGeneration::GenerateMoves(Bitboard_Util::Bitboard pieceBoard, Color color) {
    // iterate over the bitboard , for each isolated bit find the corresponding moves.
    while(pieceBoard != 0){
        uint8_t fromSquareIndex = GetLSBIndex(pieceBoard);

        // Get pre calculated attack bitboard for the current position.
        Bitboard attacks = Attacks::pawnAttacks[color][fromSquareIndex];
        while(attacks != 0){
            uint8_t toSquareIndex = GetLSBIndex(attacks);

            auto [xf , yf] = GetCoordinates(fromSquareIndex);
            auto [xt , yt] = GetCoordinates(toSquareIndex);
            Move move;

            printf("[%s%s]->[%s%s]\n",
                    FileToString((File)xf).c_str(),
                    RankToString((Rank)yf).c_str(),
                    FileToString((File)xt).c_str(),
                    RankToString((Rank)yt).c_str());

            attacks = PopBit(attacks, toSquareIndex);
        }

        pieceBoard = PopBit(pieceBoard, fromSquareIndex);
    }

}