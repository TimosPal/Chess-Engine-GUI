#ifndef FENPARSER_H
#define FENPARSER_H

#include "EngineUtil.h"

#include <tuple>
#include <sstream>

namespace ChessEngine::FenParser{ // TODO: Split into cpp file.

    std::tuple<PieceType, Color> TokenToPiece(char fenToken){
        PieceType type;
        switch(tolower(fenToken)){
            case 'p': type = PieceType::Pawn; break;
            case 'r': type = PieceType::Rook; break;
            case 'n': type = PieceType::Knight; break;
            case 'k': type = PieceType::King; break;
            case 'q': type = PieceType::Queen; break;
            case 'b': type = PieceType::Bishop; break;
            default: return {}; // Wrong fen format.
        }

        // Lowercase is Black , whereas uppercase is White.
        Color color = islower(fenToken) ? Color::Black : Color::White;
        return {type, color};
    }

    void ParseFenPlacement(const std::string& placement){

    }

    bool ParseFenString(const std::string& fenString){
        // A form of tokenizing fenString with a space delimiter.
        auto iss = std::istringstream{fenString};

        int count = 0;
        auto subStr = std::string{};
        while (iss >> subStr) {
            // A fen string is split into 6 parts.
            switch(count){
                case 0: ParseFenPlacement(subStr); break;
                case 1: break;
                case 2: break; // TODO: castling
                case 3: break; // TODO: en passant
                case 4: break; // TODO: half move number.
                case 5: break; // TODO: Full move number.
                default: return false; // We got more than 6 parts.
            }

            count++;
        }

        return count == 6; // We got less than 6 parts.
    }

}

#endif