#include "Game.h"

#include <SFML/Window/Event.hpp>

#include <Engine/MoveGeneration/PseudoMoves.h>
#include <Engine/MoveGeneration/MoveGeneration.h>

#include "./RenderingUtil.h"

namespace ChessFrontend {

        Game::Game(ChessEngine::BoardState state, bool whiteAI, bool blackAI, int width, int height, const std::string& title)
                : window(sf::VideoMode(width, height), title) ,
                board(state) , boardHasChanged(true), whiteAI(whiteAI), blackAI(blackAI)
        {
            window.setFramerateLimit(60);
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

            window.display();
        }

        void Game::PlayMove(){
            using namespace ChessEngine::MoveGeneration;

            bool isAI = (board.GetState().turnOf == ChessEngine::Color::White) ? whiteAI : blackAI;

            if(isAI){
                auto moves = Pseudo::GetAllMoves(board.GetState(), board.GetState().turnOf, board.GetUtilities());
                MakeMove(moves.front(), board.GetState().turnOf, board.GetState(), board.GetUtilities());
            }else{
                // Player code.
            }

            boardHasChanged = true;
        }

}