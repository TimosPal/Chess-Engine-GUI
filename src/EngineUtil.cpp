#include "EngineUtil.h"

std::string ChessEngine::FileToString(File file){
    switch (file){
        case File::A : return "A";
        case File::B : return "B";
        case File::C : return "C";
        case File::D : return "D";
        case File::E : return "E";
        case File::F : return "F";
        case File::G : return "G";
        case File::H : return "H";
        default: return "error";
    }
}

std::string ChessEngine::RankToString(Rank file){
    return std::string(1, file + 1 + '0');
}