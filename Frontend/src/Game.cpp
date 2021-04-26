#include "Game.h"

#include <SFML/Window/Event.hpp>

#include <Engine/MoveGeneration/PseudoMoves.h>
#include <Engine/MoveGeneration/MoveGeneration.h>
#include <unistd.h>

#include "./RenderingUtil.h"
#include "TextureManager.h"

namespace ChessFrontend {

        Game::Game(ChessEngine::BoardState state, bool whiteAI, bool blackAI, int width, int height, const std::string& title)
                : window(sf::VideoMode(width, height), title) ,
                board(state), boardHasChanged(true), whiteAI(whiteAI), blackAI(blackAI),
                isHolding(false), activePiece(false)
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
                std::cout << std::endl;
            }

            // SFML rendering.
            window.clear();

            RenderingUtil::DrawCheckerBoard(window);
            RenderingUtil::DrawPieces(window, board.GetState(), isHolding, fromPos);

            if(activePiece) {
                RenderingUtil::DrawActivePieceMoves(window, activePieceMoves);
                if (isHolding)
                    RenderingUtil::DrawHoldingPiece(window, holdingSprite);
            }

            window.display();
        }

        void Game::PlayMove(){
            using namespace ChessEngine::MoveGeneration;

            boardHasChanged = false; // Reset in every loop. Becomes true for one loop every move.

            bool isAI = (board.GetState().turnOf == ChessEngine::Color::White) ? whiteAI : blackAI;
            boardHasChanged = (isAI) ? AiTurn() : HumanTurn();
        }

        bool Game::AiTurn(){
            using namespace ChessEngine::MoveGeneration;

            auto moves = Pseudo::GetAllMoves(board.GetState(), board.GetState().turnOf, board.GetUtilities());
            MakeMove(moves.front(), board.GetState().turnOf, board.GetState(), board.GetUtilities());

            return true; // Plays move instantly.
        }

        bool Game::HumanTurn(){
            using namespace ChessEngine::BitboardUtil;
            using namespace ChessEngine::MoveGeneration;

            bool madeMove = false;

            sf::Vector2i tileSize(window.getSize().x / 8, window.getSize().y / 8);
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2i tilePos(mousePos.x / tileSize.x, 8 - 1 - mousePos.y / tileSize.y);

            uint8_t fromIndex = GetSquareIndex(fromPos.x, fromPos.y);
            uint8_t toIndex = GetSquareIndex(tilePos.x, tilePos.y);

            auto[type, color] = board.GetState().GetPosType(GetSquareIndex(tilePos.x, tilePos.y));

            if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
                // If already holding , dont do anything.
                if(!isHolding){
                    Move move{};
                    bool validMove = activePiece && IndecesToMove(fromIndex, toIndex, activePieceMoves, move);
                    if (validMove) {
                        // Clicked a move when a piece was already active and it's also valid
                        MakeMove(move, board.GetState().turnOf, board.GetState(), board.GetUtilities());

                        activePiece = false;
                        isHolding = false;
                        madeMove = true;
                    } else if (type == ChessEngine::PieceType::None || color != board.GetState().turnOf) {
                        // Clicked an invalid move , empty tile.
                        activePiece = false;
                        isHolding = false;
                    } else if (color == board.GetState().turnOf) {
                        // Clicked another owned piece.
                        holdingSprite = TextureManager::GetPieceSprite(color, type);
                        fromPos = tilePos;

                        auto pseudoMoves = Pseudo::GetAllMoves(board.GetState(), board.GetState().turnOf,
                                                               board.GetUtilities());
                        activePieceMoves = FromIndexMoves(GetSquareIndex(fromPos.x, fromPos.y), pseudoMoves);

                        activePiece = true;
                        isHolding = true;
                    }
                }
            }else{
                if(isHolding) {
                    // Stop holding the piece , even if the move is invalid.
                    isHolding = false;

                    Move move{};
                    if(IndecesToMove(fromIndex, toIndex, activePieceMoves, move)) {
                        MakeMove(move, board.GetState().turnOf, board.GetState(), board.GetUtilities());
                        activePiece = false;
                        madeMove = true;
                    }
                }
            }

            return madeMove;
        }

}