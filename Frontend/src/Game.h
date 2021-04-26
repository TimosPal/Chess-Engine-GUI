#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <Engine/Board/Board.h>
#include <SFML/Graphics/Sprite.hpp>

#include <Engine/MoveGeneration/Move.h>

#include <list>

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

        bool whiteAI;
        bool blackAI;

        bool boardHasChanged; // Since last frame.

        // Information about the current human player.
        // Used for graphics.
        bool activePiece;
        bool isHolding;
        sf::Sprite holdingSprite;
        sf::Vector2i fromPos;
        std::list<ChessEngine::MoveGeneration::Move> activePieceMoves;

        // Returns true if player made a move
        bool HumanTurn();
        bool AiTurn();

    };

}

#endif