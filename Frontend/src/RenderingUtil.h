#ifndef RENDERING_UTIL_H
#define RENDERING_UTIL_H


#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <Engine/Board/BoardState.h>

namespace ChessFrontend::RenderingUtil {

    void DrawCheckerBoard(sf::RenderWindow &window);

    void DrawPieces(sf::RenderWindow &window, ChessEngine::BoardState state);

    void ScalePieceSprite(sf::Sprite &sprite, sf::Vector2i targetScale);

}

#endif
