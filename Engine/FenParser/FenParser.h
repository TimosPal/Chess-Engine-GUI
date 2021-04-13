#ifndef FEN_PARSER_H
#define FEN_PARSER_H

#include <string>

#include "../Board/BoardState.h"

namespace ChessEngine {
    /* Various helper functions are implemented within the .cpp file */
    bool ParseFenString(const std::string &fenString, BoardState &state);
}

#endif