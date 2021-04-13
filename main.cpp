#include <iostream>

#include "Frontend/src/TextureManager.h"
#include "Frontend/src/Game.h"

#include <Engine/FenParser/FenParser.h>

constexpr int width = 800;
constexpr int height = 800;

//TODO: change globals to static ?

int main() {
    ChessEngine::Init();
    ChessFrontend::TextureManager::Init("../Frontend/Sprites"); // NOTE: Path is from the executable.

    ChessEngine::BoardState state = {};
    std::string fenPosition = "rnb1kbnr/ppqp1ppp/2pp4/3B4/3P4/3P4/PPPQ1PPP/RN1K1BNR w kq - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    ChessFrontend::Game game(state, width, height, "Chess");
    while (game.IsRunning()) {
        game.HandleEvents();
        game.Render();
    }

    return 0;
}
