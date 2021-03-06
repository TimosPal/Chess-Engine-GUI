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
        King, Queen, Bishop, Knight, Rook, Pawn, None
    };

    enum class GameState{
        Win, Defeat, Draw, Forfiet, Playing
    };

    std::string FileToString(File file);
    std::string RankToString(Rank rank);
    std::string GameStateToString(GameState state);

    char PieceTypeToChar(PieceType type, Color color);
    char PieceTypeToChar(PieceType type);

    bool StringToCoord(const std::string& str, std::tuple<uint8_t , uint8_t>& coords);

    Color InvertColor(Color color);

    /* Init engine tables etc */
    void Init();

}

#endif
