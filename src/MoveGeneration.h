#ifndef MOVEGENERATION_H
#define MOVEGENERATION_H

#include <cstdint>

namespace ChessEngine::MoveGeneration {

    struct Move{ // TODO: maybe add another file?
        uint8_t fromSquareIndex : 3;
        uint8_t toSquareIndex : 3;
    };


}

#endif
