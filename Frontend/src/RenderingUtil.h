#ifndef RENDERING_UTIL_H
#define RENDERING_UTIL_H

#include <list>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <Engine/Board/BoardOccupancies.h>
#include <Engine/MoveGeneration/Move.h>

namespace ChessFrontend::RenderingUtil {

    void DrawCheckerBoard(sf::RenderWindow &window);

    void DrawPieces(sf::RenderWindow &window, ChessEngine::BoardOccupancies &utilities, std::vector<sf::Vector2i> ignorePos, ChessEngine::Color viewSide);

    void ScalePieceSprite(sf::Sprite &sprite, sf::Vector2i targetScale);

    void DrawHoldingPiece(sf::RenderWindow &window, sf::Sprite &holdingSprite);

    void DrawActivePieceMoves(sf::RenderWindow &window, std::list<ChessEngine::MoveGeneration::Move> activePieceMoves, ChessEngine::Color viewSide);

    bool PlayMoveAnimation(sf::RenderWindow &window, ChessEngine::MoveGeneration::Move move, ChessEngine::Color color, ChessEngine::Color sideView, float lerpTime);

    void DrawPromotionMenu(sf::RenderWindow &window, sf::Vector2i originPos, ChessEngine::Color sideView);

    void DrawCoordinates(sf::RenderWindow &window, ChessEngine::Color sideView);

    // Special helper functions for image creations.
    void DrawPiecesEncoding(sf::RenderWindow &window, ChessEngine::BoardOccupancies &utilities, ChessEngine::Color viewSide);
    void Draw_x88_Indices(sf::RenderWindow &window, bool firstHalf, ChessEngine::Color viewSide);
    void Draw_Bitboard(sf::RenderWindow &window, ChessEngine::BitboardUtil::Bitboard &board, ChessEngine::Color viewSide);
    void Draw_1dArray_Indices(sf::RenderWindow &window, ChessEngine::Color viewSide);

}

#endif
