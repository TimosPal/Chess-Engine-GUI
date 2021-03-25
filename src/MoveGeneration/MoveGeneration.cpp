#include "MoveGeneration.h"

#include <list>

#include "NonSlidingPieces.h"

using namespace ChessEngine;
using namespace ChessEngine::Bitboard_Util;
using namespace ChessEngine::MoveGeneration;

bool ChessEngine::MoveGeneration::IsOfMoveType(MoveType flags, MoveType type){
    return flags & type;
}

static std::list<Move> ExtractMoves(Bitboard moves, uint8_t fromSquareIndex, MoveType flags){
    std::list<Move> moveList;

    // iterate over the bitboard , for each isolated bit find the corresponding moves.
    while(moves != 0){
        uint8_t toSquareIndex = GetLSBIndex(moves);

        Move move = {.fromSquareIndex = fromSquareIndex,
                     .toSquareIndex = toSquareIndex,
                     .type = flags};
        moveList.emplace_back(move);

        auto [xf , yf] = GetCoordinates(fromSquareIndex);
        auto [xt , yt] = GetCoordinates(toSquareIndex);

        if(IsOfMoveType(flags, MoveType::Promotion)){
            printf("Promotion ");
        }
        if(IsOfMoveType(flags, MoveType::Capture)){
            printf("Capture ");
        }
        if(IsOfMoveType(flags, MoveType::Quiet)){
            printf("Quiet ");
        }

        printf("[%s%s]->[%s%s]\n",
               FileToString((File)xf).c_str(),
               RankToString((Rank)yf).c_str(),
               FileToString((File)xt).c_str(),
               RankToString((Rank)yt).c_str());

        moves = PopBit(moves, toSquareIndex);
    }

    return moveList;
}

void ChessEngine::MoveGeneration::GenerateMoves(const BoardState& state, Color color, const Bitboard_Util::Bitboard* occupancies) {
    Color enemyColor = InvertColor(color);
    Bitboard teamOccupancies = occupancies[color];
    Bitboard enemyOccupancies = occupancies[enemyColor];
    Bitboard globalOccupancies = occupancies[Color::Both];

    // Pawns.
    Bitboard pawnsBoard = state.pieceBoards[color][PieceType::Pawn];
    while(pawnsBoard != 0){
        uint8_t fromSquareIndex = GetLSBIndex(pawnsBoard);
        Bitboard tempPieceBoard = SetBit(BITBOARD_EMPTY, fromSquareIndex);

        // Promotion Flag.
        // If the pawn is 1 tile away , moving or
        // attacking will lead to a promotion
        MoveType promotionFlag = MoveType::None;
        if((color == Color::White && tempPieceBoard & r7_Mask) ||
            (color == Color::Black && tempPieceBoard & r2_Mask)){
            promotionFlag = MoveType::Promotion;
        }

        // Quiet moves
        auto quietMoveFlags = (MoveType)(MoveType::Quiet | promotionFlag);
        Bitboard pushes = NonSlidingPieces::GetPawnPushes(tempPieceBoard, color);
        pushes |= NonSlidingPieces::GetDoublePawnPushes(tempPieceBoard, globalOccupancies, color);

        auto m1 = ExtractMoves(pushes & ~globalOccupancies, fromSquareIndex, quietMoveFlags);

        // Captures
        auto attackMoveFlags = (MoveType)(MoveType::Capture | promotionFlag);
        Bitboard attacks = NonSlidingPieces::pawnAttacks[color][fromSquareIndex];
        auto m2 = ExtractMoves(attacks & enemyOccupancies, fromSquareIndex, attackMoveFlags);

        pawnsBoard = PopBit(pawnsBoard, fromSquareIndex);
    }

    // En passant
    //state.enPassantBoard

    // Knight / King.
    // TODO:

    // Queen / rook / bishop.
    // TODO:

}