#ifndef ENGINE_UTIL_H
#define ENGINE_UTIL_H

#include <string>
#include <tuple>

namespace ChessEngine {

    /* NOTE: Not using eum classes for easy indexing */

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
        Pawn, Rook, Bishop, Knight, King, Queen, None
    };

    std::string FileToString(File file);
    std::string RankToString(Rank rank);
    std::string PieceTypeToString(PieceType type, Color color);

    bool StringToCoord(const std::string& str, std::tuple<uint8_t , uint8_t >& coords);
    Color InvertColor(Color color);

    /* InitEngine engine tables etc */
    void InitEngine();

}

#endif
