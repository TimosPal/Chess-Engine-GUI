#include "Game.h"

#include <SFML/Window/Event.hpp>

#include <Engine/MoveGeneration/PseudoMoves.h>
#include <Engine/MoveGeneration/MoveGeneration.h>

#include "./RenderingUtil.h"
#include "TextureManager.h"

namespace ChessFrontend {

        Game::Game(ChessEngine::BoardState state, bool whiteAI, bool blackAI, int width, int height, const std::string& title)
                : window(sf::VideoMode(width, height), title) ,
                board(state) , boardHasChanged(true), whiteAI(whiteAI), blackAI(blackAI)
        {
            window.setFramerateLimit(120);
        }

        void Game::HandleEvents(){
            sf::Event event{};
            while (window.pollEvent(event)) {
                // Request for closing the window
                if (event.type == sf::Event::Closed)
                    window.close();
            }
        }

        void Game::Render(){
            // Console rendering.
            // Print only once per change.
            if(boardHasChanged) {
                board.Draw();
                boardHasChanged = false;
            }

            // SFML rendering.
            window.clear();

            RenderingUtil::DrawCheckerBoard(window);
            RenderingUtil::DrawPieces(window, board.GetState());

            if(isHolding) {
                sf::Vector2i tileSize(window.getSize().x / 8, window.getSize().y / 8);
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                RenderingUtil::ScalePieceSprite(holdingSprite, tileSize);
                holdingSprite.setPosition(mousePos.x - tileSize.x / 2, mousePos.y - tileSize.y / 2);
                window.draw(holdingSprite);
            }

            window.display();
        }

        void Game::PlayMove(){
            using namespace ChessEngine::MoveGeneration;

            bool isAI = (board.GetState().turnOf == ChessEngine::Color::White) ? whiteAI : blackAI;

            if(isAI){
                auto moves = Pseudo::GetAllMoves(board.GetState(), board.GetState().turnOf, board.GetUtilities());
                MakeMove(moves.front(), board.GetState().turnOf, board.GetState(), board.GetUtilities());
                boardHasChanged = true;
            }else{
                RealPlayerTurn();
            }
        }

        void Game::RealPlayerTurn(){
            using namespace ChessEngine::BitboardUtil;

            sf::Vector2i tileSize(window.getSize().x / 8, window.getSize().y / 8);
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2i tilePos(mousePos.x / tileSize.x, 8 - 1 - mousePos.y / tileSize.y);

            auto[type, color] = board.GetState().GetPosType(GetSquareIndex(tilePos.x, tilePos.y));

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                // Is same color , not empty square and isn't already holding a sprite.
                if (type != ChessEngine::PieceType::None && color == board.GetState().turnOf  && !isHolding) {
                    isHolding = true;
                    holdingSprite = TextureManager::GetPieceSprite(color, type);
                }
            }else{
                isHolding = false;
            }

        }

}