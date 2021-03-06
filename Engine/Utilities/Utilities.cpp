#include <cassert>
#include "Utilities.h"

#include "../MoveGeneration/SlidingPieces.h"
#include "../MoveGeneration/MoveTables.h"

namespace ChessEngine {

    std::string FileToString(File file) {
        assert(file >= 0 && file <= 7);
        return std::string(1, 'a' + file);
    }

    std::string RankToString(Rank rank) {
        assert(rank >= 0 && rank <= 7);
        return std::string(1, rank + 1 + '0');
    }

    std::string GameStateToString(GameState state){
        switch (state) {
            case GameState::Win:
                return "Win";
            case GameState::Defeat:
                return "Defeat";
            case GameState::Draw:
                return "Draw";
            case GameState::Forfiet:
                return "Forfiet";
            case GameState::Playing:
                return "Playing";
        }
    }

    Color InvertColor(Color color) { return (Color) ((color + 1) % 2); }

    bool StringToCoord(const std::string &str, std::tuple<uint8_t, uint8_t> &coords) {
        if (str.length() != 2)
            return false;

        // Is a number between 0-7
        int rankTemp = str[1] - '0' - 1;
        if (rankTemp < 0 || rankTemp > 7)
            return false;

        int fileTemp = str[0] - 'a';
        if (fileTemp < 0 || fileTemp > 7)
            return false;

        // Safe cast to Enums.
        coords = {fileTemp, rankTemp};

        return true;
    }

    char PieceTypeToChar(PieceType type, Color color) {
        if (color == Color::White) {
            switch (type) {
                case PieceType::Pawn:
                    return 'P';
                case PieceType::Rook:
                    return 'R';
                case PieceType::Knight:
                    return 'N';
                case PieceType::King:
                    return 'K';
                case PieceType::Queen:
                    return 'Q';
                case PieceType::Bishop:
                    return 'B';
                case PieceType::None:
                    return '-';
                default:
                    assert(false);
            }
        } else {
            switch (type) {
                case PieceType::Pawn:
                    return 'p';
                case PieceType::Rook:
                    return 'r';
                case PieceType::Knight:
                    return 'n';
                case PieceType::King:
                    return 'k';
                case PieceType::Queen:
                    return 'q';
                case PieceType::Bishop:
                    return 'b';
                case PieceType::None:
                    return '-';
                default:
                    assert(false);
            }
        }
    }

    char PieceTypeToChar(PieceType type) {
        return PieceTypeToChar(type, Color::Black); // if we want lowercase chars , and dont care about color.
    }

    void Init() {
        MoveGeneration::SlidingPieces::InitBlockerMasks();
        MoveGeneration::MoveTables::InitMoveTables();
    }

}