#include "EngineUtil.h"

using namespace ChessEngine;

std::string ChessEngine::FileToString(File file){
    return std::string(1, 'a' + file);
}

std::string ChessEngine::RankToString(Rank file){
    return std::string(1, file + 1 + '0');
}

Color ChessEngine::InvertColor(Color color) { return (Color)((color + 1) % 2); }

bool ChessEngine::StringToCoord(const std::string& str, std::tuple<File, Rank>& coords){
    if(str.length() != 2)
        return false;

    // Is a number between 0-7
    int fileTemp = str[1] - '0' - 1;
    if(fileTemp < 0 || fileTemp > 7)
        return false;

    int rankTemp = str[0] - 'a';
    if(rankTemp < 0 || rankTemp > 7)
        return false;

    // Safe cast to Enums.
    coords = {(File)fileTemp, (Rank)rankTemp};

    return true;
}