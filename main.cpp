#include <iostream>

#include "Frontend/src/ResourceManager.h"
#include "Frontend/src/Game.h"

#include <Engine/FenParser/FenParser.h>

constexpr int width = 800;
constexpr int height = 800;
constexpr int frameLimit = 120;

constexpr float moveTime = 0.025f;

std::string ArgumentToString(int argc, char* argv[]){
    std::string temp;
    for (int i = 1; i < argc; i++) {
        temp += argv[i] + std::string(" ");
    }

    return temp;
}

int main(int argc, char* argv[]) {
    srand((unsigned) time(0));

    ChessEngine::Init();
    ChessFrontend::ResourceManager::Init("../Frontend/sprites", "../Frontend/fonts"); // NOTE: Path is from the executable.

    ChessEngine::BoardState state = {};
    std::string fenPosition = ArgumentToString(argc, argv);
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    ChessFrontend::WindowSettings windowSettings(height, width, frameLimit, "Chess");
    ChessFrontend::Options options(false, false, moveTime, false, ChessEngine::Color::White, windowSettings);
    ChessFrontend::Game game(state, options);

    sf::Clock deltaClock;
    sf::Time dt{};
    while (game.IsRunning()) {
        game.HandleEvents();
        game.Render(dt);
        game.Update();

        dt = deltaClock.restart();
    }

    return 0;
}
