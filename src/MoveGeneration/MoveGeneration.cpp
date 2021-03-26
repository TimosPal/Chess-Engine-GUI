#include "MoveGeneration.h"

#include <list>

#include "LeaperPieces.h"

#include <iostream>
#include <cassert>

using namespace ChessEngine;
using namespace ChessEngine::Bitboard_Util;
using namespace ChessEngine::MoveGeneration;

bool ChessEngine::MoveGeneration::IsOfMoveType(MoveType flags, MoveType type){
    return flags & type;
}

std::string ChessEngine::MoveGeneration::MoveTypeToString(MoveType type){
    switch (type) {
        case MoveType::None: return "None";
        case MoveType::Quiet: return "Quiet";
        case MoveType::Capture: return "Capture";
        case MoveType::Promotion: return "Promotion";
        case MoveType::EnPassant: return "EnPassant";
        case MoveType::KingSideCastling: return "KingSideCastling";
        case MoveType::QueenSideCastling: return "QueenSideCastling";
        default: return "Error";
    }
}

std::ostream& ChessEngine::MoveGeneration::operator<<(std::ostream& out, const MoveType value){
    if(value == MoveType::None){
        out << "None";
    }else{
        uint8_t flags = value;
        while(flags != 0) { // Print each flag based on the bits
            uint8_t lsbIndex = Bitboard_Util::GetLSBIndex(flags);
            MoveType tempFlag = (MoveType)Bitboard_Util::SetBit(0, lsbIndex);

            flags = Bitboard_Util::PopBit(flags, lsbIndex);
            out << MoveTypeToString(tempFlag);
            if(flags != 0) // Dont print final space.
                out << " ";
        }
    }

    return out;
}

std::ostream& ChessEngine::MoveGeneration::operator<<(std::ostream& out, Move value){
    auto [xf , yf] = GetCoordinates(value.fromSquareIndex);
    auto [xt , yt] = GetCoordinates(value.toSquareIndex);

    out << value.flags
        << " ["
        << FileToString((File)xf)
        << RankToString((Rank)yf)
        << "]->["
        << FileToString((File)xt)
        << RankToString((Rank)yt)
        << "]";

    return out;
}


static std::list<Move> ExtractMoves(Bitboard moves, uint8_t fromSquareIndex, MoveType flags){
    std::list<Move> moveList;

    // iterate over the bitboard , for each isolated bit find the corresponding moves.
    while(moves != 0){
        uint8_t toSquareIndex = GetLSBIndex(moves);

        Move move = {.fromSquareIndex = fromSquareIndex,
                     .toSquareIndex = toSquareIndex,
                     .flags = flags};

        moveList.emplace_back(move);

        std::cout << move << std::endl;

        moves = PopBit(moves, toSquareIndex);
    }

    return moveList;
}

void ChessEngine::MoveGeneration::GeneratePseudoPawnMoves(const BoardState& state, Color color, const Bitboard_Util::Bitboard* occupancies){
    Color enemyColor = InvertColor(color);
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
        Bitboard pushes = LeaperPieces::GetPawnPushes(tempPieceBoard, color);
        pushes |= LeaperPieces::GetDoublePawnPushes(tempPieceBoard, globalOccupancies, color);

        auto m1 = ExtractMoves(pushes & ~globalOccupancies, fromSquareIndex, quietMoveFlags);

        // Captures
        auto attackMoveFlags = (MoveType)(MoveType::Capture | promotionFlag);
        Bitboard attacks = LeaperPieces::pawnAttacks[color][fromSquareIndex];
        auto m2 = ExtractMoves(attacks & enemyOccupancies, fromSquareIndex, attackMoveFlags);

        // En passant
        // NOTE: slightly waste full to check for every pawn (Only 2 needed)
        if(state.enPassantBoard != BITBOARD_EMPTY) {
            // Previous move enabled en passant.

            // NOTE: it is possible to en passant your own piece only if
            // something is wrong with the turns!
            assert(!(attacks & state.enPassantBoard & r2_Mask && color == Color::White) ||
                   !(attacks & state.enPassantBoard & r7_Mask && color == Color::Black));

            auto enPassantMoveFlags = (MoveType)(MoveType::Capture | MoveType::EnPassant);
            auto m3 = ExtractMoves(attacks & state.enPassantBoard, fromSquareIndex, enPassantMoveFlags);
        }

        pawnsBoard = PopBit(pawnsBoard, fromSquareIndex);
    }
}

void ChessEngine::MoveGeneration::GeneratePseudoCastlingMoves(const BoardState& state, Color color, const Bitboard_Util::Bitboard* occupancies){
    // Check whether or not there are pieces between the king and the rook.
    Bitboard globalOccupancies = occupancies[Color::Both];
    Bitboard colorMask = (color == Color::White) ? r1_Mask : r8_Mask;

    if(state.kingSideCastling[color]){
        bool emptyKingSide = (colorMask & kingSideCastling_Mask & globalOccupancies) == 0;
        if(emptyKingSide){
            Move move = {.fromSquareIndex = GetLSBIndex(kingsStartingPosBoard & colorMask),
                         .toSquareIndex = GetLSBIndex(kingsCastlePosBoard & colorMask),
                         .flags = MoveType::KingSideCastling};
            std::cout << move << std::endl;
        }
    }
    if(state.queenSideCastling[color]){
        bool emptyQueenSide = (colorMask & queenSideCastling_Mask & globalOccupancies) == 0;
        if(emptyQueenSide){
            Move move = {.fromSquareIndex = GetLSBIndex(kingsStartingPosBoard & colorMask),
                         .toSquareIndex = GetLSBIndex(queenCastlePosBoard & colorMask),
                         .flags = MoveType::QueenSideCastling};
            std::cout << move << std::endl;
        }
    }
}

void ChessEngine::MoveGeneration::GeneratePseudoMoves(const BoardState& state, Color color, const Bitboard_Util::Bitboard* occupancies) {
    // Pawns.
    GeneratePseudoPawnMoves(state, color, occupancies);

    // Knight / King.
    // TODO:

    // Castling.
    GeneratePseudoCastlingMoves(state, color, occupancies);

    // Queen / rook / bishop.
    // TODO:

}