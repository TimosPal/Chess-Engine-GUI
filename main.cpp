#include <iostream>

#include "Frontend/src/TextureManager.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Engine/FenParser/FenParser.h>
#include <Engine/Board/Board.h>

#include <Engine/MoveGeneration/PseudoMoves.h>
#include <Engine/MoveGeneration/MoveGeneration.h>

constexpr int width = 800;
constexpr int height = 800;

//TODO: change globals to static ?

void DrawBoard(sf::RenderWindow& window, ChessEngine::BoardState state){
    auto tileSize = sf::Vector2f(width/8, height/8);
    sf::RectangleShape rectangle(tileSize);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            rectangle.setPosition(tileSize.x * i, tileSize.y * j);
            rectangle.setFillColor((i % 2 == j % 2) ? sf::Color::White : sf::Color(40, 40, 40,255));
            window.draw(rectangle);
        }
    }

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            auto [type, color] = state.GetPosType(ChessEngine::BitboardUtil::GetSquareIndex(i,j));
            if(type == ChessEngine::PieceType::None)
                continue;

            auto currSprite = ChessFrontend::TextureManager::GetPieceSprite(color, type);
            int spritePixelSize = currSprite.getTexture()->getSize().x / 6;
            float scalingFactor =  (float)tileSize.x / spritePixelSize;

            currSprite.setScale(scalingFactor, scalingFactor);
            currSprite.setPosition(i * tileSize.x, (8 - j - 1) * tileSize.y);
            window.draw(currSprite);

        }
    }
}

int main() {
    ChessEngine::Init();
    ChessFrontend::TextureManager::Init("../Frontend/Sprites"); // NOTE: Path is from the executable.

    ChessEngine::BoardState state = {};
    std::string fenPosition = "rnbqkbnr/pppppppp/8/2Q5/3KP3/2PPB1N1/PP3PPP/RN3B1R w kq - 0 1";
    if(!ParseFenString(fenPosition, state)){
        std::cout << "Incorrect fen string" << std::endl;
        return -1;
    }

    ChessEngine::Board board(state);
    board.Draw();

    auto moves = ChessEngine::MoveGeneration::Pseudo::GetAllMoves(board.GetState(), ChessEngine::Color::White, board.GetUtilities());
    ChessEngine::MoveGeneration::PrintMoves(moves);

    sf::RenderWindow window(sf::VideoMode(width, height), "Chess");
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
            // Request for closing the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        DrawBoard(window, board.GetState());
        window.display();
    }

    return 0;
}
