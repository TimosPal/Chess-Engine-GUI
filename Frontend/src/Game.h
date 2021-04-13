#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <Engine/Board/Board.h>

namespace ChessFrontend {

    class Game{
    public:
        Game(ChessEngine::BoardState state, int width, int height, std::string title);

        void HandleEvents();
        void Render();

        bool IsRunning() { return window.isOpen();}

    private:
        sf::RenderWindow window;
        ChessEngine::Board board;
    };

}

#endif