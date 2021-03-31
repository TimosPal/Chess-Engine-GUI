#include "FenParser.h"

#include <tuple>
#include <sstream>

using namespace ChessEngine;

/*******************************************************/
/* Private functions                                   */
/*******************************************************/

/* Translate a char token to a {pieceType, Color} */
static bool TokenToPiece(char fenToken, std::tuple<PieceType, Color>& pieceInfo){
    PieceType type;
    switch(tolower(fenToken)){
        case 'p': type = PieceType::Pawn; break;
        case 'r': type = PieceType::Rook; break;
        case 'n': type = PieceType::Knight; break;
        case 'k': type = PieceType::King; break;
        case 'q': type = PieceType::Queen; break;
        case 'b': type = PieceType::Bishop; break;
        default: return false; // Wrong fen token.
    }

    // Lowercase is Black , whereas uppercase is White.
    Color color = islower(fenToken) ? Color::Black : Color::White;
    pieceInfo = {type, color};
    return true;
}

/* Translate a string representing piece positions */
static bool ParseFenPlacement(const std::string& placement, BoardState& state){
    // Rank are separated by a '/'.
    // numbers mean consecutive empty spaces.
    // letters correspond to piece types.

    std::stringstream stream(placement);
    std::string subString;
    int rank = Rank::R8; // Fen strings start from rank 8 going towards rank 1.
    while (getline(stream, subString, '/')) { // Iterate over each rank.
        int file = 0;
        for (char token : subString) {
            if(isdigit(token)){ // Skip the empty squares - 1 (due to file++)
                int emptySquares = token - '0';
                file += emptySquares - 1;
            }else {
                // Get the piece info based on the fen token.
                std::tuple<PieceType, Color> pieceInfo;
                if(!TokenToPiece(token, pieceInfo))
                    return false;
                auto [type, color] = pieceInfo;

                // Update appropriate bitboard with the piece position.
                BitboardUtil::Bitboard &currPieceBoard = state.pieceBoards[color][type];
                uint8_t squareIndex = BitboardUtil::GetSquareIndex(file, rank);
                currPieceBoard |= BitboardUtil::SetBit(currPieceBoard, squareIndex);
            }
            file++;

            if(file > File::H + 1) // Check for possible overshoots.
                return false;
        }

        if(file < File::H + 1) // If a rank isn't fully described.
            return false;

        rank--;
    }

    return rank == Rank::R1 - 1; // All 8 rank were described (no more or less).
}

/* Translate a string to a team Color , representing the turn color */
static bool ParseFenTurn(const std::string& turn, BoardState& state){
    if(turn.size() == 1){ // Should be just a single char
        switch(turn[0]){
            case 'w': state.turnOf = Color::White; return true;
            case 'b': state.turnOf = Color::Black; return true;
            default: return false; // Wrong token.
        }
    }else{
        return false;
    }
}

/* Translate the castling rights */
static bool ParseCastlingRights(const std::string& rightsString, BoardState& state){
    // KQkq means every castling move is available , - none.
    // NOTE: doesn't decline strings with wrong ordering (eg: qKQk)

    int stringLen = rightsString.length();
    if(stringLen == 0 || stringLen > 4)
        return false;
    if(rightsString == "-"){
        state.kingSideCastling[Color::White] = false;
        state.kingSideCastling[Color::Black] = false;
        state.queenSideCastling[Color::White] = false;
        state.queenSideCastling[Color::Black] = false;
        return true;
    }

    for(char token : rightsString) {
        std::tuple<PieceType, Color> pieceInfo;
        if(!TokenToPiece(token, pieceInfo))
            return false;
        auto [type, color] = pieceInfo;

        switch (type) {
            case PieceType::King:
                if(!state.kingSideCastling[color])
                    state.kingSideCastling[color] = true;
                else
                    return false; // Already set.
                break;
            case PieceType::Queen:
                if(!state.queenSideCastling[color])
                    state.queenSideCastling[color] = true;
                else
                    return false; // Already set.
                break;
            default: return false; // Wrong token.
        }
    }

    return true;
}

/* Translates half move or full move strings */
static bool ParseMoveCounter(const std::string& counterString, BoardState& state, bool forHalf){
    // create an input stream with your string.
    // NOTE: number concat with another string still passes!
    int counter;
    std::istringstream iss (counterString);
    iss >> counter;
    if (iss.fail()) {
        return false;
    }
    if(counter < 0){
        return false;
    }

    if(forHalf){
        state.halfMoves = counter;
    }else{
        state.fullMoves = counter;
    }

    return true;
}

/* Translates en passant strings */
static bool ParseEnPassant(const std::string& enPassantString, BoardState& state){
    if(enPassantString == "-"){
        state.enPassantBoard = BITBOARD_EMPTY;
        return true;
    }

    std::tuple<File, Rank> coords;
    if(!StringToCoord(enPassantString, coords))
        return false;

    auto [file, rank] = coords;
    uint8_t squareIndex = BitboardUtil::GetSquareIndex(file, rank);
    state.enPassantBoard = BitboardUtil::SetBit(BITBOARD_EMPTY, squareIndex);

    return true;
}

/*******************************************************/
/* Public functions                                    */
/*******************************************************/

bool ChessEngine::ParseFenString(const std::string& fenString, BoardState& state){
    BoardState tempState = state;
    std::stringstream stream(fenString);

    int count = 0;
    std::string subString;
    while (getline(stream, subString, ' ')) {
        // A fen string is split into 6 parts.
        switch(count){
            case 0: if(!ParseFenPlacement(subString, tempState)) return false; break;
            case 1: if(!ParseFenTurn(subString, tempState)) return false; break;
            case 2: if(!ParseCastlingRights(subString, tempState)) return false; break;
            case 3: if(!ParseEnPassant(subString, tempState)) return false; break;
            case 4: if(!ParseMoveCounter(subString, tempState, true)) return false; break;
            case 5: if(!ParseMoveCounter(subString, tempState, false)) return false; break;
            default: return false; // We got more than 6 parts.
        }

        count++;
    }

    if(count == 6) { // We got all 6 parts of the fen format.
        state = tempState; // Only alter the state if the string was successfully parsed.
        return true;
    }else{
        return false;
    }
}