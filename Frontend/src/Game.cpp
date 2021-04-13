#include "Game.h"

#include <SFML/Window/Event.hpp>

#include "./RenderingUtil.h"

namespace ChessFrontend {

        Game::Game(ChessEngine::BoardState state, int width, int height, std::string title)
                : window(sf::VideoMode(width, height), title) , board(state)
        {
            window.setFramerateLimit(60);
        }

        void Game::HandleEvents(){
            sf::Event event{};
            while (window.pollEvent(event)) {
                // Request for closing the window
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }

        void Game::Render(){
            // Console rendering.
            //board.Draw();

            // SFML rendering.
            window.clear();

            RenderingUtil::DrawCheckerBoard(window);
            RenderingUtil::DrawPieces(window, board.GetState());

            window.display();
        }

}