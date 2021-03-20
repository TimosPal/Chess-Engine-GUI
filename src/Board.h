#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

#include "BoardState.h"
#include "FenParser.h"

#include <iostream>

namespace ChessEngine {

    class Board {
    public:
        explicit Board(const std::string& fenString) {
            if(ParseFenString(fenString, state)){
                std::cout << "Correct" << std::endl;
            }else{
                std::cout << "Incorrect" << std::endl;
            }
        }

        void Draw(); // Prints the Board on the console.
    private:

        BoardState state{};
    };

}

#endif