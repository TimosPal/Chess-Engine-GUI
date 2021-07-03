#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <Engine/Board/Board.h>
#include <SFML/Graphics/Sprite.hpp>

#include <Engine/MoveGeneration/Move.h>

#include "Options.h"
#include "HumanState.h"

#include <list>

namespace ChessFrontend {

    class Game {
    public:
        Game(ChessEngine::BoardState state, const Options& options);

        void HandleEvents();
        void Update();
        void Render(sf::Time dt);

        bool IsRunning() { return window.isOpen(); }

    private:
        sf::RenderWindow window;
        ChessEngine::Board board;
        Options options;

        // Used for human move selection.
        // Describes current active player.
        // Not used for AIs.
        HumanState humanState;

        // Used for printing the board in the console
        // or playing move animations.
        bool boardHasChanged; // Since last frame.
        bool shouldMoveAnimation;
        bool playMoveAnimation;

        float elapsedAnimTime;

        // Used for handling single clicks.
        bool singleClick;
        bool clickRelease;

        bool DropPieceMove(uint8_t fromIndex, uint8_t toIndex);
        bool ClickPieceMove(uint8_t fromIndex, uint8_t toIndex, sf::Vector2i tilePos);
        bool PickMove(sf::Vector2i tilePos);
        bool PickPromotion(ChessEngine::Color color, sf::Vector2i tilePos);

        // Returns true if player made a move
        bool HumanTurn();
        bool AiTurn(); // Handles AI moves.

        std::vector<sf::Vector2i> GetIgnoreList();
        void SwapSides();

        void PlayMove();


    };

}

#endif