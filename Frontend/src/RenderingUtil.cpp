#include "RenderingUtil.h"

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/CircleShape.hpp>

#include "./TextureManager.h"

#define BLACK_TILE_COLOR 238, 238, 210
#define WHITE_TILE_COLOR 118, 150, 86

#define CIRCLE_COLOR 0, 0, 0, 50
#define CIRCLE_QUIET_PERCENTAGE 0.20
#define CIRCLE_ATTACK_PERCENTAGE 1.0
#define CIRCLE_OUTLINE_PERCENTAGE 0.07

namespace ChessFrontend::RenderingUtil {

    void DrawCheckerBoard(sf::RenderWindow &window) {
        int width = window.getSize().x;
        int height = window.getSize().y;

        auto tileSize = sf::Vector2f(width / 8, height / 8);
        sf::RectangleShape rectangle(tileSize);

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                rectangle.setPosition(tileSize.x * i, tileSize.y * j);
                rectangle.setFillColor((i % 2 == j % 2) ? sf::Color(BLACK_TILE_COLOR) : sf::Color(WHITE_TILE_COLOR));
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

    void DrawActivePieceMoves(sf::RenderWindow &window, const std::list<ChessEngine::MoveGeneration::Move>& activePieceMoves){
        using namespace ChessEngine::MoveGeneration;
        using namespace ChessEngine::BitboardUtil;

        sf::Vector2i tileSize(window.getSize().x / 8, window.getSize().y / 8);
        float circleRadius = tileSize.x / 2;
        float quietRadius = CIRCLE_QUIET_PERCENTAGE * circleRadius;
        float captureRadius = CIRCLE_ATTACK_PERCENTAGE * circleRadius;

        sf::CircleShape shape(circleRadius);
        shape.setOutlineThickness(- tileSize.x * CIRCLE_OUTLINE_PERCENTAGE);

        for(auto move : activePieceMoves) {
            auto [posX , posY] = GetCoordinates(move.toSquareIndex);

            // Captures have a ring circle.
            float currentRadius;
            if(IsMoveType(move.flags, MoveType::Capture) && !IsMoveType(move.flags, MoveType::EnPassant)){
                currentRadius = captureRadius;
                shape.setFillColor(sf::Color::Transparent);
                shape.setOutlineColor(sf::Color(CIRCLE_COLOR));
            }else{
                currentRadius = quietRadius;
                shape.setFillColor(sf::Color(CIRCLE_COLOR));
                shape.setOutlineColor(sf::Color::Transparent);
            }

            // Tile positions.
            float newPosX = tileSize.x * posX;
            float newPosY = tileSize.y * (8 - posY - 1);

            // Center circle.
            newPosX = newPosX + tileSize.x / 2 - currentRadius;
            newPosY = newPosY + tileSize.y / 2 - currentRadius;

            shape.setPosition(newPosX, newPosY);
            shape.setRadius(currentRadius);
            window.draw(shape);
        }
    }

}