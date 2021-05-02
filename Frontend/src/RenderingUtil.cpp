#include "RenderingUtil.h"

#include <vector>

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
        int width = window.getView().getSize().x;
        int height = window.getView().getSize().y;

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

    void DrawPieces(sf::RenderWindow &window, ChessEngine::BoardUtilities& utilities, std::vector<sf::Vector2i> ignorePos) {
        int width = window.getView().getSize().x;
        int height = window.getView().getSize().y;
        auto tileSize = sf::Vector2(width / 8, height / 8);

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                auto[type, color] = utilities.squaresOccupants[ChessEngine::BitboardUtil::GetSquareIndex(i, j)];
                if (type == ChessEngine::PieceType::None) // Empty tile.
                    continue;

                auto findCurrPos = std::find(ignorePos.begin(), ignorePos.end(), sf::Vector2i(i,j));
                if(findCurrPos != ignorePos.end()){
                    continue;
                }

                auto currSprite = ChessFrontend::TextureManager::GetPieceSprite(color, type);
                ScalePieceSprite(currSprite, tileSize);
                currSprite.setPosition(i * tileSize.x, (8 - j - 1) * tileSize.y);

                window.draw(currSprite);
            }
        }
    }

    void DrawHoldingPiece(sf::RenderWindow& window, sf::Sprite& holdingSprite){
        sf::Vector2i tileSize(window.getView().getSize().x / 8, window.getView().getSize().y / 8);
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);

        RenderingUtil::ScalePieceSprite(holdingSprite, tileSize);
        holdingSprite.setPosition(worldPos.x - tileSize.x / 2, worldPos.y - tileSize.y / 2);
        window.draw(holdingSprite);
    }

    void DrawActivePieceMoves(sf::RenderWindow &window, std::list<ChessEngine::MoveGeneration::Move> activePieceMoves){
        using namespace ChessEngine::MoveGeneration;
        using namespace ChessEngine::BitboardUtil;

        sf::Vector2i tileSize(window.getView().getSize().x / 8, window.getView().getSize().y / 8);
        float circleRadius = tileSize.x / 2;
        float quietRadius = CIRCLE_QUIET_PERCENTAGE * circleRadius;
        float captureRadius = CIRCLE_ATTACK_PERCENTAGE * circleRadius;

        sf::CircleShape shape(circleRadius);
        shape.setOutlineThickness(- tileSize.x * CIRCLE_OUTLINE_PERCENTAGE);

        // Remove move entries with same destination (Should be promotion moves) , so we get no overlapping circles.
        auto binaryPredicate = [] (Move m1, Move m2) { return m1.toSquareIndex == m2.toSquareIndex; };
        activePieceMoves.unique(binaryPredicate);

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

    float LerpF(float a, float b, float t){
        if(t <= 0)
            t = 0;
        if(t >= 1)
            t = 1;

        return a + t*(b-a);
    }

    bool MoveAnimation(sf::RenderWindow &window, uint8_t fromIndex, uint8_t toIndex, sf::Sprite sprite, float lerpTime){
        using namespace ChessEngine::BitboardUtil;

        sf::Vector2i tileSize(window.getView().getSize().x / 8, window.getView().getSize().y / 8);

        auto [fromX , fromY] = GetCoordinates(fromIndex);
        auto [toX , toY] = GetCoordinates(toIndex);

        float xVal = LerpF(fromX, toX, lerpTime);
        float yVal = LerpF(fromY, toY, lerpTime);

        ScalePieceSprite(sprite, tileSize);
        sprite.setPosition(xVal * tileSize.x, (8 - yVal - 1) * tileSize.y);

        window.draw(sprite);

        return (xVal != toX || yVal != toY); // Returns false when the position is reached.
    }

    bool FadeAnimation(sf::RenderWindow &window, uint8_t posIndex, sf::Sprite sprite, float lerpTime){
        using namespace ChessEngine::BitboardUtil;

        sf::Vector2i tileSize(window.getView().getSize().x / 8, window.getView().getSize().y / 8);
        auto [posX , posY] = GetCoordinates(posIndex);

        auto tempColor = sprite.getColor();
        tempColor.a = LerpF(255, 0, lerpTime);
        sprite.setColor(tempColor);

        ScalePieceSprite(sprite, tileSize);
        sprite.setPosition(posX * tileSize.x, (8 - posY - 1) * tileSize.y);

        window.draw(sprite);
    }

    bool PlayMoveAnimation(sf::RenderWindow &window, ChessEngine::MoveGeneration::Move move, ChessEngine::Color color, float lerpTime){
        using namespace ChessEngine::MoveGeneration;
        using namespace ChessEngine::BitboardUtil;
        // Returns false if animation is done.

        ChessEngine::Color moveColor = ChessEngine::InvertColor(color);

        auto [toX , toY] = GetCoordinates(move.toSquareIndex);

        if(IsMoveType(move.flags, MoveType::Capture)) {
            float enemyX = toX , enemyY = toY;
            if(IsMoveType(move.flags, MoveType::EnPassant)) {
                // Find en passant piece.
                enemyY += moveColor == ChessEngine::Color::White ? -1 : 1;
            }

            auto enemySprite = ChessFrontend::TextureManager::GetPieceSprite(color, move.enemyType);
            FadeAnimation(window, GetSquareIndex(enemyX, enemyY), enemySprite, lerpTime);
        }

        auto selfSprite = ChessFrontend::TextureManager::GetPieceSprite(moveColor, move.selfType);
        return MoveAnimation(window, move.fromSquareIndex, move.toSquareIndex, selfSprite, lerpTime);
    }

}