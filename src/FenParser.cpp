#include "FenParser.h"

#include <tuple>
#include <sstream>

#include "EngineUtil.h"
#include "Bitboard.h"

using namespace ChessEngine;

/*******************************************************/
/* Private functions                                   */
/*******************************************************/

static std::tuple<PieceType, Color> TokenToPiece(char fenToken){
    PieceType type;
    switch(tolower(fenToken)){
        case 'p': type = PieceType::Pawn; break;
        case 'r': type = PieceType::Rook; break;
        case 'n': type = PieceType::Knight; break;
        case 'k': type = PieceType::King; break;
        case 'q': type = PieceType::Queen; break;
        case 'b': type = PieceType::Bishop; break;
        default: return {}; // Wrong fen format. // TODO: handle that case in higher calls.
    }

    // Lowercase is Black , whereas uppercase is White.
    Color color = islower(fenToken) ? Color::Black : Color::White;
    return {type, color};
}

static void ParseFenPlacement(const std::string& placement, BoardState& state){
    int count = 0;
    for(char token : placement){
        if(token == '/') continue; // '/' separates ranks.

        auto [type, color] = TokenToPiece(token);
        Bitboard_Util::Bitboard& currPieceBoard = state.pieceBoards[color][type];
        currPieceBoard |= Bitboard_Util::SetBit(currPieceBoard, count);

        count++;
    }
}

/*******************************************************/
/* Public functions                                    */
/*******************************************************/

bool ChessEngine::ParseFenString(const std::string& fenString, BoardState& state){
    // A form of tokenizing fenString with a space delimiter.
    auto iss = std::istringstream{fenString};

    int count = 0;
    auto subStr = std::string{};
    while (iss >> subStr) {
        // A fen string is split into 6 parts.
        switch(count){
            case 0: ParseFenPlacement(subStr, state); break;
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