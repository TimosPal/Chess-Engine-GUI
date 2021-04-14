#include "RenderingUtil.h"

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

    void DrawPieces(sf::RenderWindow &window, ChessEngine::BoardState state) {
        int width = window.getSize().x;
        int height = window.getSize().y;
        auto tileSize = sf::Vector2(width / 8, height / 8);

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                auto[type, color] = state.GetPosType(ChessEngine::BitboardUtil::GetSquareIndex(i, j));
                if (type == ChessEngine::PieceType::None) // Empty tile.
                    continue;

                auto currSprite = ChessFrontend::TextureManager::GetPieceSprite(color, type);
                int spritePixelSize = currSprite.getTexture()->getSize().x / 6;
                float scalingFactor = ChessFrontend::TextureManager::GetScalingFactor(tileSize.x, spritePixelSize);

                currSprite.setScale(scalingFactor, scalingFactor);
                currSprite.setPosition(i * tileSize.x, (8 - j - 1) * tileSize.y);

                window.draw(currSprite);
            }
        }
    }

}