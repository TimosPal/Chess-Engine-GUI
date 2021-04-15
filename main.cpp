#include <iostream>

#include "Frontend/src/TextureManager.h"
#include "Frontend/src/Game.h"

#include <Engine/FenParser/FenParser.h>
#include <unistd.h>

constexpr int width = 800;
constexpr int height = 800;

int main() {
    ChessEngine::Init();
    ChessFrontend::TextureManager::Init("../Frontend/Sprites"); // NOTE: Path is from the executable.

    ChessEngine::BoardState state = {};
    std::string fenPosition = "rnbqkbnr/pppppppp/8/7P/1BP2P2/2PQ2P1/PP2P3/RN2KBNR w KQkq - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    ChessFrontend::Game game(state,false, true, width, height, "Chess");
    while (game.IsRunning()) {
        game.HandleEvents();
        game.PlayMove();
        game.Render();
    }

    return 0;
}
