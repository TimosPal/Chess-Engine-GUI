#include "RenderingUtil.h"

#include <SFML/Window/Mouse.hpp>

#include "./TextureManager.h"

namespace ChessFrontend::RenderingUtil {

    void DrawCheckerBoard(sf::RenderWindow &window) {
        int width = window.getSize().x;
        int height = window.getSize().y;

        auto tileSize = sf::Vector2f(width / 8, height / 8);
        sf::RectangleShape rectangle(tileSize);

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                rectangle.setPosition(tileSize.x * i, tileSize.y * j);
                rectangle.setFillColor((i % 2 == j % 2) ? sf::Color(238, 238, 210) : sf::Color(118, 150, 86));
                window.draw(rectangle);
            }
        }
    }

    void ScalePieceSprite(sf::Sprite& sprite, sf::Vector2i targetScale){
        int spritePixelSize = sprite.getTexture()->getSize().x / 6;
        float scalingFactor = ChessFrontend::TextureManager::GetScalingFactor(targetScale.x, spritePixelSize);
        sprite.setScale(scalingFactor, scalingFactor);
    }

    void DrawPieces(sf::RenderWindow &window, ChessEngine::BoardState state, bool isHolding, sf::Vector2i fromPos) {
        int width = window.getSize().x;
        int height = window.getSize().y;
        auto tileSize = sf::Vector2(width / 8, height / 8);

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                auto[type, color] = state.GetPosType(ChessEngine::BitboardUtil::GetSquareIndex(i, j));
                if (type == ChessEngine::PieceType::None) // Empty tile.
                    continue;
                if (isHolding && fromPos.x == i && fromPos.y == j) // holding piece
                    continue;

                auto currSprite = ChessFrontend::TextureManager::GetPieceSprite(color, type);
                ScalePieceSprite(currSprite, tileSize);
                currSprite.setPosition(i * tileSize.x, (8 - j - 1) * tileSize.y);

                window.draw(currSprite);
            }
        }
    }

    void DrawHoldingPiece(sf::RenderWindow& window, sf::Sprite& holdingSprite){
        sf::Vector2i tileSize(window.getSize().x / 8, window.getSize().y / 8);
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        RenderingUtil::ScalePieceSprite(holdingSprite, tileSize);
        holdingSprite.setPosition(mousePos.x - tileSize.x / 2, mousePos.y - tileSize.y / 2);
        window.draw(holdingSprite);
    }

}