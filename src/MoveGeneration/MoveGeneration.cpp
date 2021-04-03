#include "MoveGeneration.h"

#include <list>
#include <iostream>
#include <cassert>

#include "MoveTables.h"
#include "LeaperPieces.h"

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
                return "Error";
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

        out << value.flags
            << " ["
            << FileToString((File) xf)
            << RankToString((Rank) yf)
            << "]->["
            << FileToString((File) xt)
            << RankToString((Rank) yt)
            << "]";

        return out;
    }

    static std::list<Move> ExtractMoves(Bitboard moves, uint8_t fromSquareIndex, MoveType flags) {
        std::list<Move> moveList;

        // iterate over the bitboard , for each isolated bit find the corresponding moves.
        while (moves != 0) {
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

    void GetPseudoPawnMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = occupancies[enemyColor];
        Bitboard globalOccupancies = occupancies[Color::Both];

        // Pawns.
        Bitboard pawnsBoard = state.pieceBoards[color][PieceType::Pawn];
        while (pawnsBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(pawnsBoard);
            Bitboard tempPieceBoard = SetBit(BITBOARD_EMPTY, fromSquareIndex);

            // Promotion Flag.
            // If the pawn is 1 tile away , moving or
            // attacking will lead to a promotion
            MoveType promotionFlag = MoveType::None;
            if ((color == Color::White && tempPieceBoard & r7_Mask) ||
                (color == Color::Black && tempPieceBoard & r2_Mask)) {
                promotionFlag = MoveType::Promotion;
            }

            // Quiet moves
            auto quietMoveFlags = (MoveType) (MoveType::Quiet | promotionFlag);
            Bitboard pushes = LeaperPieces::GetPawnPushes(tempPieceBoard, color);
            pushes |= LeaperPieces::GetDoublePawnPushes(tempPieceBoard, globalOccupancies, color);

            auto m1 = ExtractMoves(pushes & ~globalOccupancies, fromSquareIndex, quietMoveFlags);

            // Captures
            auto attackMoveFlags = (MoveType) (MoveType::Capture | promotionFlag);
            Bitboard attacks = MoveTables::GetPawnAttacks(color, fromSquareIndex);
            auto m2 = ExtractMoves(attacks & enemyOccupancies, fromSquareIndex, attackMoveFlags);

            // En passant
            // NOTE: slightly waste full to check for every pawn (Only 2 needed)
            if (state.enPassantBoard != BITBOARD_EMPTY) {
                // Previous move enabled en passant.

                // NOTE: it is possible to en passant your own piece only if
                // something is wrong with the turns!
                assert(!(attacks & state.enPassantBoard & r2_Mask && color == Color::White) ||
                       !(attacks & state.enPassantBoard & r7_Mask && color == Color::Black));

                auto enPassantMoveFlags = (MoveType) (MoveType::Capture | MoveType::EnPassant);
                auto m3 = ExtractMoves(attacks & state.enPassantBoard, fromSquareIndex, enPassantMoveFlags);
            }

            pawnsBoard = PopBit(pawnsBoard, fromSquareIndex);
        }
    }

    void GetPseudoCastlingMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        // Check whether or not there are pieces between the king and the rook.
        Bitboard globalOccupancies = occupancies[Color::Both];
        Bitboard colorMask = (color == Color::White) ? r1_Mask : r8_Mask;

        if (state.kingSideCastling[color]) {
            bool emptyKingSide = (colorMask & kingSideCastling_Mask & globalOccupancies) == 0;
            if (emptyKingSide) {
                Move move = {.fromSquareIndex = GetLSBIndex(kingsStartingPosBoard & colorMask),
                        .toSquareIndex = GetLSBIndex(kingsCastlePosBoard & colorMask),
                        .flags = MoveType::KingSideCastling};
                std::cout << move << std::endl;
            }
        }
        if (state.queenSideCastling[color]) {
            bool emptyQueenSide = (colorMask & queenSideCastling_Mask & globalOccupancies) == 0;
            if (emptyQueenSide) {
                Move move = {.fromSquareIndex = GetLSBIndex(kingsStartingPosBoard & colorMask),
                        .toSquareIndex = GetLSBIndex(queenCastlePosBoard & colorMask),
                        .flags = MoveType::QueenSideCastling};
                std::cout << move << std::endl;
            }
        }
    }

    void GetPseudoKnightMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = occupancies[enemyColor];
        Bitboard globalOccupancies = occupancies[Color::Both];

        // Pawns.
        Bitboard knightsBoard = state.pieceBoards[color][PieceType::Knight];
        while (knightsBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(knightsBoard);

            Bitboard moves = MoveTables::GetKnightMoves(fromSquareIndex);

            // Quiet moves
            auto m1 = ExtractMoves(moves & ~globalOccupancies, fromSquareIndex, MoveType::Quiet);

            // Captures
            auto m2 = ExtractMoves(moves & enemyOccupancies, fromSquareIndex, MoveType::Capture);

            knightsBoard = PopBit(knightsBoard, fromSquareIndex);
        }
    }

    void GetPseudoKingMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = occupancies[enemyColor];
        Bitboard globalOccupancies = occupancies[Color::Both];

        Bitboard kingBoard = state.pieceBoards[color][PieceType::King];

        if (kingBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(kingBoard);
            Bitboard moves = MoveTables::GetKingMoves(fromSquareIndex);

            // Quiet moves
            auto m1 = ExtractMoves(moves & ~globalOccupancies, fromSquareIndex, MoveType::Quiet);

            // Captures
            auto m2 = ExtractMoves(moves & enemyOccupancies, fromSquareIndex, MoveType::Capture);
        }
    }

    void GetPseudoSlidingPieceMoves(const BoardState &state, Color color, const Bitboard *occupancies, PieceType type) {
        Color enemyColor = InvertColor(color);
        Bitboard enemyOccupancies = occupancies[enemyColor];
        Bitboard globalOccupancies = occupancies[Color::Both];

        Bitboard slidingPieceBoard = state.pieceBoards[color][type];

        Bitboard (*getMoves)(uint8_t, Bitboard);
        switch (type) { // Find proper function for piece type.
            case PieceType::Queen: getMoves = MoveTables::GetQueenMoves; break;
            case PieceType::Rook: getMoves = MoveTables::GetRookMoves; break;
            case PieceType::Bishop: getMoves = MoveTables::GetBishopMoves; break;
            default: assert(false); // Invalid
        }

        while (slidingPieceBoard != 0) {
            uint8_t fromSquareIndex = GetLSBIndex(slidingPieceBoard);

            Bitboard moves = getMoves(fromSquareIndex, globalOccupancies);

            // Quiet moves
            auto m1 = ExtractMoves(moves & ~globalOccupancies, fromSquareIndex, MoveType::Quiet);

            // Captures
            auto m2 = ExtractMoves(moves & enemyOccupancies, fromSquareIndex, MoveType::Capture);

            slidingPieceBoard = PopBit(slidingPieceBoard, fromSquareIndex);
        }
    }

    void GetPseudoMoves(const BoardState &state, Color color, const Bitboard *occupancies) {
        // Pawns.
        GetPseudoPawnMoves(state, color, occupancies);
        // King.
        GetPseudoKingMoves(state, color, occupancies);
        // Knight
        GetPseudoKnightMoves(state, color, occupancies);
        // Castling.
        GetPseudoCastlingMoves(state, color, occupancies);
        // Queen
        GetPseudoSlidingPieceMoves(state, color, occupancies, PieceType::Rook);
        // Bishop
        GetPseudoSlidingPieceMoves(state, color, occupancies, PieceType::Bishop);
        // Queen.
        GetPseudoSlidingPieceMoves(state, color, occupancies, PieceType::Queen);
    }

}