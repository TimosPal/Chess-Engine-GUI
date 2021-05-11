#include <iostream>

#include "Frontend/src/TextureManager.h"
#include "Frontend/src/Game.h"

#include <Engine/FenParser/FenParser.h>

constexpr int width = 800;
constexpr int height = 800;

constexpr float moveTime = 0.25f;

int main() {
    srand((unsigned) time(0));

    ChessEngine::Init();
    ChessFrontend::TextureManager::Init("../Frontend/Sprites"); // NOTE: Path is from the executable.

    ChessEngine::BoardState state = {};
    std::string fenPosition = "5k2/8/3q1q2/8/8/8/8/R3K2R w KQ - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    ChessFrontend::Game game(state, false, true, moveTime, width, height, "Chess");

    sf::Clock deltaClock;
    sf::Time dt{};
    while (game.IsRunning()) {
        game.HandleEvents();
        game.Render(dt);
        game.PlayMove();

        dt = deltaClock.restart();
    }

    return 0;
}
