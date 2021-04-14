#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <Engine/Board/Board.h>

namespace ChessFrontend {

    class Game{
    public:
        Game(ChessEngine::BoardState state, bool whiteAI, bool blackAI, int width, int height, const std::string& title);

        void HandleEvents();
        void PlayMove();
        void Render();

        bool IsRunning() { return window.isOpen(); }

    private:
        sf::RenderWindow window;
        ChessEngine::Board board;

        bool boardHasChanged; // Since last frame.

        bool whiteAI;
        bool blackAI;
    };

}

#endif