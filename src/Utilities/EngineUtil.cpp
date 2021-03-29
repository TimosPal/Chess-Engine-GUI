#include <cassert>
#include "EngineUtil.h"

using namespace ChessEngine;

std::string ChessEngine::FileToString(File file){
    assert(file >= 0 && file <= 7);
    return std::string(1, 'a' + file);
}

std::string ChessEngine::RankToString(Rank rank){
    assert(rank >= 0 && rank <= 7);
    return std::string(1, rank + 1 + '0');
}

Color ChessEngine::InvertColor(Color color) { return (Color)((color + 1) % 2); }

bool ChessEngine::StringToCoord(const std::string& str, std::tuple<File, Rank>& coords){
    if(str.length() != 2)
        return false;

    // Is a number between 0-7
    int rankTemp = str[1] - '0' - 1;
    if(rankTemp < 0 || rankTemp > 7)
        return false;

    int fileTemp = str[0] - 'a';
    if(fileTemp < 0 || fileTemp > 7)
        return false;

    // Safe cast to Enums.
    coords = {(File)fileTemp, (Rank)rankTemp};

    return true;
}

const char* ChessEngine::PieceTypeToAscii(PieceType type, Color color){
    if(color == Color::White) {
        switch (type) {
            case PieceType::Pawn:
                return "P";
            case PieceType::Rook:
                return "R";
            case PieceType::Knight:
                return "N";
            case PieceType::King:
                return "K";
            case PieceType::Queen:
                return "Q";
            case PieceType::Bishop:
                return "B";
            default:
                return "-";
        }
    }else{
        switch (type) {
            case PieceType::Pawn:
                return "p";
            case PieceType::Rook:
                return "r";
            case PieceType::Knight:
                return "n";
            case PieceType::King:
                return "k";
            case PieceType::Queen:
                return "q";
            case PieceType::Bishop:
                return "b";
            default:
                return "-";
        }
    }
}