#ifndef ENGINEUTIL_H
#define ENGINEUTIL_H

#include <string>

namespace ChessEngine {

    enum Color {
        White, Black, Both
    };

    enum File{
        A, B, C, D, E, F, G, H
    };

    enum Rank{
        R1, R2, R3, R4, R5, R6, R7, R8
    };

    enum PieceType{
        Pawn, Rook, Bishop, Knight, King, Queen
    };

    std::string FileToString(File file);
    std::string RankToString(Rank file);

    Color InvertColor(Color color);

}

#endif
