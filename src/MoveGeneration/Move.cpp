#include "Move.h"

#include <iostream>
#include <cassert>

#include "../Board/Bitboard.h"

namespace ChessEngine::MoveGeneration {

    using namespace ChessEngine::BitboardUtil;

    bool IsOfMoveType(MoveType flags, MoveType type) {
        return flags & type;
    }

    std::string MoveTypeToString(MoveType type) {
        switch (type) {
            case MoveType::None:
                return "None";
            case MoveType::Quiet:
                return "Quiet";
            case MoveType::Capture:
                return "Capture";
            case MoveType::Promotion:
                return "Promotion";
            case MoveType::EnPassant:
                return "EnPassant";
            case MoveType::KingSideCastling:
                return "KingSideCastling";
            case MoveType::QueenSideCastling:
                return "QueenSideCastling";
            default:
                assert(false);
        }
    }

    std::ostream &operator<<(std::ostream &out, const MoveType value) {
        if (value == MoveType::None) {
            out << "None";
        } else {
            uint8_t flags = value;
            while (flags != 0) { // Print each flag based on the bits
                uint8_t lsbIndex = GetLSBIndex(flags);
                auto tempFlag = (MoveType) SetBit(0, lsbIndex);

                flags = PopBit(flags, lsbIndex);
                out << MoveTypeToString(tempFlag);
                if (flags != 0) // Dont print final space.
                    out << " ";
            }
        }

        return out;
    }

    std::ostream &operator<<(std::ostream &out, Move value) {
        auto[xf, yf] = GetCoordinates(value.fromSquareIndex);
        auto[xt, yt] = GetCoordinates(value.toSquareIndex);

        // Empty string if None.
        std::string enemyTypeStr;
        if(value.enemyType != PieceType::None){
            enemyTypeStr = PieceTypeToChar(value.enemyType);
        }

        out << value.flags
            << " [ "
            << PieceTypeToChar(value.selfType)
            << FileToString((File) xf)
            << RankToString((Rank) yf)
            << " -> "
            << enemyTypeStr
            << FileToString((File) xt)
            << RankToString((Rank) yt)
            << " ]";

        return out;
    }

}
