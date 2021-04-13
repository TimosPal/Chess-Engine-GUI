#ifndef RENDERINGUTIL_H
#define RENDERINGUTIL_H


#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <Engine/Board/BoardState.h>

namespace ChessFrontend::RenderingUtil {

    void DrawCheckerBoard(sf::RenderWindow& window);
    void DrawPieces(sf::RenderWindow& window, ChessEngine::BoardState state);

}

#endif
