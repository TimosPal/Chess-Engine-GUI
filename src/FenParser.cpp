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

static void ParseFenPlacement(const std::string& placement, BoardState& state){ // TODO: error checking.
    // Rank are separated by a '/'.
    // numbers mean consecutive empty spaces.
    // letters correspond to piece types.

    std::stringstream stream(placement);
    std::string subString;
    int rank = Rank::R8; // Fen strings start from rank 8 going towards rank 1.
    while (getline(stream, subString, '/')) { // Iterate over each rank.
        int file = 0;
        for (char token : subString) {
            if(isdigit(token)){ // Need to only update the file counter.
                int emptySquares = token - '0'; // Convert from ascii to int.
                file += emptySquares;
            }else {
                // Update appropriate bitboard with the piece position.
                auto[type, color] = TokenToPiece(token);
                Bitboard_Util::Bitboard &currPieceBoard = state.pieceBoards[color][type];
                uint8_t squareIndex = Bitboard_Util::GetSquareIndex(file, rank);
                currPieceBoard |= Bitboard_Util::SetBit(currPieceBoard, squareIndex);
            }
            file++;
        }
        rank--;
    }
}

/*******************************************************/
/* Public functions                                    */
/*******************************************************/

bool ChessEngine::ParseFenString(const std::string& fenString, BoardState& state){ // TODO: error checking.
    std::stringstream stream(fenString);

    int count = 0;
    std::string subString;
    while (getline(stream, subString, ' ')) {
        // A fen string is split into 6 parts.
        switch(count){
            case 0: ParseFenPlacement(subString, state); break;
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