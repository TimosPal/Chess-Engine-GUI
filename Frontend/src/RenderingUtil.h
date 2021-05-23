#ifndef RENDERING_UTIL_H
#define RENDERING_UTIL_H

#include <list>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <Engine/Board/BoardUtilities.h>
#include <Engine/MoveGeneration/Move.h>

namespace ChessFrontend::RenderingUtil {

    void DrawCheckerBoard(sf::RenderWindow &window);

    void DrawPieces(sf::RenderWindow &window, ChessEngine::BoardUtilities &utilities, std::vector<sf::Vector2i> ignorePos, ChessEngine::Color viewSide);

    void ScalePieceSprite(sf::Sprite &sprite, sf::Vector2i targetScale);

    void DrawHoldingPiece(sf::RenderWindow &window, sf::Sprite &holdingSprite);

    void DrawActivePieceMoves(sf::RenderWindow &window, std::list<ChessEngine::MoveGeneration::Move> activePieceMoves, ChessEngine::Color viewSide);

    bool PlayMoveAnimation(sf::RenderWindow &window, ChessEngine::MoveGeneration::Move move, ChessEngine::Color color, ChessEngine::Color sideView, float lerpTime);

    void DrawPromotionMenu(sf::RenderWindow &window, sf::Vector2i originPos, ChessEngine::Color sideView);

}

#endif
