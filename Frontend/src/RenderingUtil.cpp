#include "RenderingUtil.h"

#include <vector>

#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>

#include "./ResourceManager.h"

#define BLACK_TILE_COLOR 238, 238, 210
#define WHITE_TILE_COLOR 118, 150, 86

#define CIRCLE_COLOR 0, 0, 0, 50
#define CIRCLE_QUIET_PERCENTAGE 0.20
#define CIRCLE_ATTACK_PERCENTAGE 1.0
#define CIRCLE_OUTLINE_PERCENTAGE 0.07

#define FONT_SIZE_PERCENTAGE 0.16
#define FONT_OFFSET_PERCENTAGE 0.03

#define SHADE_OPACITY 0.35

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
        float scalingFactor = ChessFrontend::ResourceManager::GetScalingFactor(targetScale.x, spritePixelSize);
        sprite.setScale(scalingFactor, scalingFactor);
    }

    void DrawPieces(sf::RenderWindow &window, ChessEngine::BoardOccupancies& utilities, std::vector<sf::Vector2i> ignorePos, ChessEngine::Color viewSide) {
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

                auto currSprite = ChessFrontend::ResourceManager::GetPieceSprite(color, type);
                ScalePieceSprite(currSprite, tileSize);
                // IsDraw based on viewSide view.
                int yPos = viewSide == ChessEngine::Color::White ? 7 - j : j;
                int xPos = viewSide == ChessEngine::Color::White ? i : 7 - i;
                currSprite.setPosition(xPos * tileSize.x, yPos * tileSize.y);

                window.draw(currSprite);
            }
        }
    }

    void DrawPiecesEncoding(sf::RenderWindow &window, ChessEngine::BoardOccupancies &utilities, ChessEngine::Color viewSide) {
        // Helper function for images.
        // Draw the encoded types of the pieces for the array representations.
        int width = window.getView().getSize().x;
        int height = window.getView().getSize().y;
        auto tileSize = sf::Vector2(width / 8, height / 8);

        sf::Text text;
        text.setFont(ResourceManager::GetFont());
        int fontSize = tileSize.x * 0.4;
        text.setCharacterSize(fontSize);


        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                auto[type, color] = utilities.squaresOccupants[ChessEngine::BitboardUtil::GetSquareIndex(i, j)];

                // IsDraw based on viewSide view.
                int yPos = viewSide == ChessEngine::Color::White ? 7 - j : j;
                int xPos = viewSide == ChessEngine::Color::White ? i : 7 - i;

                sf::Color textColor = sf::Color::Black;
                int pieceNum = color == ChessEngine::White ? type : type + 6;
                if (type == ChessEngine::PieceType::None) { // Empty tile.
                    textColor.a = SHADE_OPACITY * 255;
                    pieceNum = 6*2;
                }
                text.setString(std::to_string(pieceNum));

                //center text
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

                text.setFillColor(textColor);
                text.setPosition(xPos * tileSize.x + tileSize.x/2, yPos * tileSize.y + tileSize.y/2);

                window.draw(text);
            }
        }
    }

    void Draw_x88_Indices(sf::RenderWindow &window, bool firstHalf, ChessEngine::Color viewSide) {
        // Helper function for images.
        // Draws the x88 indices.
        int width = window.getView().getSize().x;
        int height = window.getView().getSize().y;
        auto tileSize = sf::Vector2(width / 8, height / 8);

        sf::Text text;
        text.setFont(ResourceManager::GetFont());
        int fontSize = tileSize.x * 0.4;
        text.setCharacterSize(fontSize);
        sf::Color textColor = sf::Color::Black;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // IsDraw based on viewSide view.
                int yPos = viewSide == ChessEngine::Color::White ? 7 - j : j;
                int xPos = viewSide == ChessEngine::Color::White ? i : 7 - i;

                int boardOffset = firstHalf ? 8 : 0;
                uint8_t positionIndex = (i + boardOffset) | (j << 4);
                text.setString(std::to_string(positionIndex));

                //center text
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

                text.setFillColor(textColor);
                text.setPosition(xPos * tileSize.x + tileSize.x/2, yPos * tileSize.y + tileSize.y/2);

                window.draw(text);
            }
        }
    }

    void Draw_1dArray_Indices(sf::RenderWindow &window, ChessEngine::Color viewSide) {
        // Helper function for images.
        // Draws the x88 indices.
        int width = window.getView().getSize().x;
        int height = window.getView().getSize().y;
        auto tileSize = sf::Vector2(width / 8, height / 8);

        sf::Text text;
        text.setFont(ResourceManager::GetFont());
        int fontSize = tileSize.x * 0.4;
        text.setCharacterSize(fontSize);
        sf::Color textColor = sf::Color::Black;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // IsDraw based on viewSide view.
                int yPos = viewSide == ChessEngine::Color::White ? 7 - j : j;
                int xPos = viewSide == ChessEngine::Color::White ? i : 7 - i;

                uint8_t positionIndex = ChessEngine::BitboardUtil::GetSquareIndex(i, j);
                text.setString(std::to_string(positionIndex));

                //center text
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

                text.setFillColor(textColor);
                text.setPosition(xPos * tileSize.x + tileSize.x/2, yPos * tileSize.y + tileSize.y/2);

                window.draw(text);
            }
        }
    }

    void Draw_Bitboard(sf::RenderWindow &window, ChessEngine::BitboardUtil::Bitboard &board, ChessEngine::Color viewSide) {
        // Helper function for images.
        // Draws the provided bitboard.
        int width = window.getView().getSize().x;
        int height = window.getView().getSize().y;
        auto tileSize = sf::Vector2(width / 8, height / 8);

        sf::Text text;
        text.setFont(ResourceManager::GetFont());
        int fontSize = tileSize.x * 0.4;
        text.setCharacterSize(fontSize);
        sf::Color textColor1 = sf::Color::Black;
        sf::Color textColor0 = sf::Color::Black;
        textColor0.a = SHADE_OPACITY * 255;

        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                // IsDraw based on viewSide view.
                int yPos = viewSide == ChessEngine::Color::White ? 7 - j : j;
                int xPos = viewSide == ChessEngine::Color::White ? i : 7 - i;

                uint8_t index = ChessEngine::BitboardUtil::GetSquareIndex(i, j);
                auto is0 = ChessEngine::BitboardUtil::GetBit(board, index) == 0;
                text.setString(is0 ? "0" : "1");

                //center text
                sf::FloatRect textRect = text.getLocalBounds();
                text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top  + textRect.height/2.0f);

                text.setFillColor(is0 ? textColor0 : textColor1);
                text.setPosition(xPos * tileSize.x + tileSize.x/2, yPos * tileSize.y + tileSize.y/2);

                window.draw(text);
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

    void DrawPromotionMenu(sf::RenderWindow &window, sf::Vector2i originPos, ChessEngine::Color sideView){
        using namespace ChessEngine;

        sf::Vector2i tileSize(window.getView().getSize().x / 8, window.getView().getSize().y / 8);

        // Find color side based on from pos.
        Color color = originPos.y == 7 ? Color::White : Color::Black;

        // IsDraw shade.
        sf::RectangleShape shade(window.getView().getSize());
        sf::Color shadeColor = sf::Color::Black;
        shadeColor.a = SHADE_OPACITY * 255;
        shade.setFillColor(shadeColor);
        window.draw(shade);

        // IsDraw basic window.
        sf::RectangleShape rectangle(sf::Vector2f(tileSize.x, tileSize.y * 4));

        int rectX = sideView == Color::White ? originPos.x : 7 - originPos.x;
        int rectY = sideView == color ? 0 : (8 - 4) * tileSize.y;

        rectangle.setPosition(rectX * tileSize.x, rectY);
        rectangle.setFillColor(sf::Color::White);
        rectangle.setOutlineThickness(- tileSize.x * 0.05);
        rectangle.setOutlineColor(sf::Color::Black);

        window.draw(rectangle);

        // IsDraw each piece option.
        PieceType options[] = {PieceType::Queen, PieceType::Knight, PieceType::Rook, PieceType::Bishop};
        for (int i = 0; i < 4; i++) {
            auto currSprite = ChessFrontend::ResourceManager::GetPieceSprite(color, options[i]);
            ScalePieceSprite(currSprite, tileSize);

            int yPos = (sideView == color ? i : (7-i)) * tileSize.y;
            currSprite.setPosition(rectX * tileSize.x, yPos);

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

            auto enemySprite = ChessFrontend::ResourceManager::GetPieceSprite(color, move.enemyType);
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

            auto rookSprite = ChessFrontend::ResourceManager::GetPieceSprite(moveColor, ChessEngine::PieceType::Rook);
            TransformAnimation(window, rookOldIndex, rookNewIndex, rookSprite, lerpTime, sideView);
        }

        auto selfSprite = ChessFrontend::ResourceManager::GetPieceSprite(moveColor, move.selfType);
        return TransformAnimation(window, move.fromSquareIndex, move.toSquareIndex, selfSprite, lerpTime, sideView);
    }

    void DrawCoordinates(sf::RenderWindow &window, ChessEngine::Color sideView){
        int width = window.getView().getSize().x;
        int height = window.getView().getSize().y;

        auto tileSize = sf::Vector2f(width / 8, height / 8);

        sf::Text text;
        text.setFont(ResourceManager::GetFont());
        int fontSize = tileSize.x * FONT_SIZE_PERCENTAGE;
        text.setCharacterSize(fontSize);

        // Ranks.
        for (int i = 0; i < 8; i++) {
            int tilePos = (sideView == ChessEngine::Color::Black) ? i : 7 - i;
            std::string numStr = ChessEngine::RankToString((ChessEngine::Rank)tilePos);
            text.setString(numStr);
            text.setFillColor((i % 2 == 0 % 2) ? sf::Color(WHITE_TILE_COLOR) : sf::Color(BLACK_TILE_COLOR));

            int posX = tileSize.x * FONT_OFFSET_PERCENTAGE;
            int posY = tileSize.x * i + tileSize.y * FONT_OFFSET_PERCENTAGE;

            text.setPosition(posX, posY);
            window.draw(text);
        }

        // Files.
        for (int i = 0; i < 8; i++) {
            int tilePos = (sideView == ChessEngine::Color::Black) ? 7 - i : i;
            std::string numStr = ChessEngine::FileToString((ChessEngine::File)tilePos);
            text.setString(numStr);
            text.setFillColor((i % 2 != 0 % 2) ? sf::Color(WHITE_TILE_COLOR) : sf::Color(BLACK_TILE_COLOR));

            int posX = (tileSize.x * (i + 1)) - fontSize - tileSize.x * FONT_OFFSET_PERCENTAGE;
            int posY = 8 * tileSize.y - fontSize - tileSize.y * FONT_OFFSET_PERCENTAGE;

            text.setPosition(posX, posY);
            window.draw(text);
        }
    }

}