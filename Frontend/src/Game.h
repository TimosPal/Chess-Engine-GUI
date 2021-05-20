#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <Engine/Board/Board.h>
#include <SFML/Graphics/Sprite.hpp>

#include <Engine/MoveGeneration/Move.h>

#include "Options.h"

#include <list>

namespace ChessFrontend {

    class Game {
    public:
        Game(ChessEngine::BoardState state, const Options& options);

        void HandleEvents();

        void PlayMove();

        void Render(sf::Time dt);

        bool IsRunning() { return window.isOpen(); }

    private:
        sf::RenderWindow window;
        ChessEngine::Board board;

        Options options;

        bool boardHasChanged; // Since last frame.

        // Information about the current human player.
        // Used for graphics.
        bool activePiece;
        bool isHolding;
        bool shouldMoveAnimation;
        bool playMoveAnimation;
        bool promotionMenu;

        sf::Sprite holdingSprite;
        sf::Vector2i fromPos;
        float elapsedAnimTime;

        ChessEngine::MoveGeneration::Move lastPlayedMove;
        std::list<ChessEngine::MoveGeneration::Move> activePieceMoves;

        bool DropPieceMove(uint8_t fromIndex, uint8_t toIndex);
        bool ClickPieceMove(uint8_t fromIndex, uint8_t toIndex, sf::Vector2i tilePos);
        bool PickMove(uint8_t fromIndex, uint8_t toIndex, sf::Vector2i tilePos);
        bool PickPromotion(ChessEngine::Color color);

        ChessEngine::PieceType PromotionSelection();

        // Returns true if player made a move
        bool HumanTurn();
        bool AiTurn();

        std::vector<sf::Vector2i> GetIgnoreList();


    };

}

#endif