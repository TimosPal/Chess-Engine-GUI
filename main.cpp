#include <iostream>

#include "Frontend/src/TextureManager.h"
#include "Frontend/src/Game.h"

#include <Engine/FenParser/FenParser.h>

constexpr int width = 800;
constexpr int height = 800;
constexpr int frameLimit = 120;

constexpr float moveTime = 0.25f;

int main() {
    srand((unsigned) time(0));

    ChessEngine::Init();
    ChessFrontend::TextureManager::Init("../Frontend/Sprites"); // NOTE: Path is from the executable.

    ChessEngine::BoardState state = {};
    std::string fenPosition = "8/3P4/3K4/8/8/5k2/5p2/8 w - - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    ChessFrontend::WindowSettings windowSettings(height, width, frameLimit, "Chess");
    ChessFrontend::Options options(true, true, moveTime, windowSettings);
    ChessFrontend::Game game(state, options);

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
