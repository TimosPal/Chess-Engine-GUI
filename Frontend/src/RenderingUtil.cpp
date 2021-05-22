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

    void DrawPieces(sf::RenderWindow &window, ChessEngine::BoardUtilities& utilities, std::vector<sf::Vector2i> ignorePos, ChessEngine::Color viewSide) {
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
                // Draw based on viewSide view.
                int yPos = viewSide == ChessEngine::Color::White ? 7 - j : j;
                int xPos = viewSide == ChessEngine::Color::White ? i : 7 - i;
                currSprite.setPosition(xPos * tileSize.x, yPos * tileSize.y);

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

    void DrawActivePieceMoves(sf::RenderWindow &window, std::list<ChessEngine::MoveGeneration::Move> activePieceMoves, ChessEngine::Color viewSide){
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

            // Based on color side view.
            // Tile positions.
            float newPosX = (viewSide == ChessEngine::Color::White ? posX : 7 - posX) * tileSize.x;
            float newPosY = (viewSide == ChessEngine::Color::White ? 7 - posY : posY) * tileSize.y;

            // Center circle.
            newPosX = newPosX + tileSize.x / 2 - currentRadius;
            newPosY = newPosY + tileSize.y / 2 - currentRadius;

            shape.setPosition(newPosX, newPosY);
            shape.setRadius(currentRadius);
            window.draw(shape);
        }
    }

    void DrawPromotionMenu(sf::RenderWindow &window, sf::Vector2i fromPos){ // Make it work for both sides
        using namespace ChessEngine;

        // TODO : fix spawn position for attacks to be based on to not form.

        sf::Vector2i tileSize(window.getView().getSize().x / 8, window.getView().getSize().y / 8);

        // Find color side based on from pos.
        Color color = fromPos.y == 7 ? Color::White : Color::Black;

        // Draw basic window.
        sf::RectangleShape rectangle(sf::Vector2f(tileSize.x, tileSize.y * 4));

        int rectY = color == Color::White ? 0 : (8 - 4) * tileSize.y;
        rectangle.setPosition(fromPos.x * tileSize.x, rectY);
        rectangle.setFillColor(sf::Color::White);
        rectangle.setOutlineThickness(- tileSize.x * 0.05);
        rectangle.setOutlineColor(sf::Color::Black);

        window.draw(rectangle);

        // Draw each piece option.
        PieceType options[] = {PieceType::Queen, PieceType::Knight, PieceType::Rook, PieceType::Bishop};
        for (int i = 0; i < 4; i++) {
            auto currSprite = ChessFrontend::TextureManager::GetPieceSprite(color, options[i]);
            ScalePieceSprite(currSprite, tileSize);

            int yPos;
            if(color == Color::White){
                yPos = i * tileSize.y;
            }else{
                yPos = (8 -1 -i) * tileSize.y;
            }

            currSprite.setPosition(fromPos.x * tileSize.x, yPos);

            window.draw(currSprite);
        }
    }

    float LerpF(float a, float b, float t){
        if(t <= 0)
            t = 0;
        if(t >= 1)
            t = 1;

        return a + t*(b-a);
    }

    bool TransformAnimation(sf::RenderWindow &window, uint8_t fromIndex, uint8_t toIndex, sf::Sprite sprite, float lerpTime, ChessEngine::Color sideView){
        using namespace ChessEngine::BitboardUtil;

        sf::Vector2i tileSize(window.getView().getSize().x / 8, window.getView().getSize().y / 8);

        auto [fromX , fromY] = GetCoordinates(fromIndex);
        auto [toX , toY] = GetCoordinates(toIndex);

        if(sideView == ChessEngine::Color::Black){
            // Needed for animations to work with side flipping.
            toX = 7 - toX;
            toY = 7 - toY;
            fromX = 7 - fromX;
            fromY = 7 - fromY;
        }

        float xVal = LerpF(fromX, toX, lerpTime);
        float yVal = LerpF(fromY, toY, lerpTime);

        ScalePieceSprite(sprite, tileSize);
        sprite.setPosition(xVal * tileSize.x, (7 - yVal) * tileSize.y);

        window.draw(sprite);

        return (xVal != toX || yVal != toY); // Returns false when the position is reached.
    }

    bool FadeAnimation(sf::RenderWindow &window, uint8_t posIndex, sf::Sprite sprite, float lerpTime, ChessEngine::Color sideView){
        using namespace ChessEngine::BitboardUtil;

        sf::Vector2i tileSize(window.getView().getSize().x / 8, window.getView().getSize().y / 8);
        auto [posX , posY] = GetCoordinates(posIndex);

        if(sideView == ChessEngine::Color::Black){
            // Needed for animations to work with side flipping.
            posX = 7 - posX;
            posY = 7 - posY;
        }

        auto tempColor = sprite.getColor();
        tempColor.a = LerpF(255, 0, lerpTime);
        sprite.setColor(tempColor);

        ScalePieceSprite(sprite, tileSize);
        sprite.setPosition(posX * tileSize.x, (8 - posY - 1) * tileSize.y);

        window.draw(sprite);

        return tempColor.a != 0;
    }

    bool PlayMoveAnimation(sf::RenderWindow &window, ChessEngine::MoveGeneration::Move move, ChessEngine::Color color, ChessEngine::Color sideView, float lerpTime){
        using namespace ChessEngine::MoveGeneration;
        using namespace ChessEngine::BitboardUtil;
        // Returns false if animation is done.
        // NOTE: color is the opposite color of the move's color.

        ChessEngine::Color moveColor = ChessEngine::InvertColor(color);

        auto [toX , toY] = GetCoordinates(move.toSquareIndex);

        if(IsMoveType(move.flags, MoveType::Capture)) {
            float enemyX = toX , enemyY = toY;
            if(IsMoveType(move.flags, MoveType::EnPassant)) {
                // Find en passant piece.
                enemyY += moveColor == ChessEngine::Color::White ? -1 : 1;
            }

            auto enemySprite = ChessFrontend::TextureManager::GetPieceSprite(color, move.enemyType);
            FadeAnimation(window, GetSquareIndex(enemyX, enemyY), enemySprite, lerpTime, sideView);
        }
        if(IsMoveType(move.flags, (MoveType)(MoveType::QueenSideCastling | MoveType::KingSideCastling))){
            uint8_t rookNewIndex;
            bool kingSide;
            if(IsMoveType(move.flags, MoveType::QueenSideCastling)){
                rookNewIndex = move.toSquareIndex + 1;
                kingSide = false;
            }else{
                rookNewIndex = move.toSquareIndex - 1;
                kingSide = true;
            }
            uint8_t rookOldIndex = GetStartingRookIndex(moveColor, kingSide);

            auto rookSprite = ChessFrontend::TextureManager::GetPieceSprite(moveColor, ChessEngine::PieceType::Rook);
            TransformAnimation(window, rookOldIndex, rookNewIndex, rookSprite, lerpTime, sideView);
        }

        auto selfSprite = ChessFrontend::TextureManager::GetPieceSprite(moveColor, move.selfType);
        return TransformAnimation(window, move.fromSquareIndex, move.toSquareIndex, selfSprite, lerpTime, sideView);
    }



}