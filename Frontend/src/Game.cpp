#include "Game.h"

#include <SFML/Window/Event.hpp>

#include <Engine/MoveGeneration/MoveGeneration.h>

#include "./RenderingUtil.h"
#include "TextureManager.h"

namespace ChessFrontend {

        Game::Game(ChessEngine::BoardState state, const Options& options)
                : window(sf::VideoMode(
                         options.windowSettings.width,
                         options.windowSettings.height),
                         options.windowSettings.title),
                         board(state), options(options)
        {
            playMoveAnimation = false;
            shouldMoveAnimation = false;
            boardHasChanged = true;

            elapsedAnimTime = 0.0f;

            window.setFramerateLimit(options.windowSettings.frameLimit);
        }

        void Game::HandleEvents(){
            singleClick = false;
            clickRelease = false;

            sf::Event event{};
            while (window.pollEvent(event)) {
                // Request for closing the window
                if (event.type == sf::Event::Closed)
                    window.close();
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left)
                        singleClick = true;
                }
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left)
                        clickRelease = true;
                }
            }
        }

        void Game::Render(sf::Time dt){
            // Console rendering.
            // Print only once per change.
            if(boardHasChanged) {
                board.Draw();
                std::cout << std::endl;
            }

            // SFML rendering.
            window.clear();

            RenderingUtil::DrawCheckerBoard(window);
            RenderingUtil::DrawPieces(window, board.GetUtilities(), GetIgnoreList());

            if(humanState.promotionMenu){
                RenderingUtil::DrawPromotionMenu(window, humanState.fromPos);
            }

            // Draw available moves and the holding piece.
            if(humanState.activePiece) {
                RenderingUtil::DrawActivePieceMoves(window, humanState.activePieceMoves);
                if (humanState.isHolding)
                    RenderingUtil::DrawHoldingPiece(window, humanState.holdingSprite);
            }

            // Handle the move animations.
            if(playMoveAnimation) {
                float lerpTime = elapsedAnimTime / options.secPerMove;
                ChessEngine::Color turnOf = board.GetState().turnOf;
                playMoveAnimation = RenderingUtil::PlayMoveAnimation(window, humanState.selectedMove, turnOf, lerpTime);

                if(playMoveAnimation)
                    elapsedAnimTime += dt.asSeconds();
                else
                    elapsedAnimTime = 0;
            }

            window.display();
        }

        std::vector<sf::Vector2i> Game::GetIgnoreList(){
            // A list with ignored positions when drawing pieces.
            // Useful for when playing animations , or holding a piece.
            using namespace ChessEngine::MoveGeneration;

            std::vector<sf::Vector2i> ignoreList = {};
            if(humanState.isHolding)
                ignoreList.push_back(humanState.fromPos);
            if(playMoveAnimation) {
                // If animation is playing exclude end position from showing.
                auto [toX, toY] = ChessEngine::BitboardUtil::GetCoordinates(humanState.selectedMove.toSquareIndex);
                ignoreList.emplace_back(toX, toY);
            }
            if(IsMoveType(humanState.selectedMove.flags, (MoveType)(MoveType::QueenSideCastling | MoveType::KingSideCastling))){
                // If castling move , exclude rook move during animation.
                uint8_t rookNewIndex = IsMoveType(humanState.selectedMove.flags, MoveType::QueenSideCastling) ?
                                       humanState.selectedMove.toSquareIndex + 1 : humanState.selectedMove.toSquareIndex - 1;

                auto [toX, toY] = ChessEngine::BitboardUtil::GetCoordinates(rookNewIndex);
                ignoreList.emplace_back(toX, toY);
            }

            return ignoreList;
        }

        void Game::PlayMove(){
            using namespace ChessEngine::MoveGeneration;

            boardHasChanged = false; // Reset in every loop. Becomes true for one loop every move.

            if(playMoveAnimation)
                return;

            bool isAI = (board.GetState().turnOf == ChessEngine::Color::White) ? options.whiteAI : options.blackAI;
            boardHasChanged = (isAI) ? AiTurn() : HumanTurn();
        }

        bool Game::AiTurn(){
            using namespace ChessEngine::MoveGeneration;

            auto moves = GetValidMoves(board.GetState(), board.GetState().turnOf, board.GetUtilities());

            Move mv;
            int rng = rand() % moves.size();
            int i = 0;
            for(auto m : moves){
                if(i == rng){
                    mv = m;
                    break;
                }

                i++;
            }

            MakeMove(mv, board.GetState().turnOf, board.GetState(), board.GetUtilities());

            humanState.selectedMove = mv;
            playMoveAnimation = true;

            return true; // Plays move instantly.
        }

        bool Game::HumanTurn(){
            using namespace ChessEngine::BitboardUtil;
            using namespace ChessEngine::MoveGeneration;

            bool shouldMove;
            if(!humanState.promotionMenu) {
                shouldMove = PickMove();
            }else{
                shouldMove = PickPromotion(board.GetState().turnOf);
            }

            if(shouldMove){
                MakeMove(humanState.selectedMove, board.GetState().turnOf, board.GetState(), board.GetUtilities());
                playMoveAnimation = shouldMoveAnimation;
                return true;
            }else{
                return false;
            }
        }

        bool Game::PickPromotion(ChessEngine::Color color){
            if(singleClick) {
                // We need to find the appropriate menu bounds based on the color.
                humanState.promotionMenu = false;
            }

            return false;
        }

        bool Game::PickMove(){
            using namespace ChessEngine::BitboardUtil;
            using namespace ChessEngine::MoveGeneration;

            sf::Vector2i tileSize(window.getSize().x / 8, window.getSize().y / 8);
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2i tilePos(mousePos.x / tileSize.x, 8 - 1 - mousePos.y / tileSize.y);

            uint8_t fromIndex = GetSquareIndex(humanState.fromPos.x, humanState.fromPos.y);
            uint8_t toIndex = GetSquareIndex(tilePos.x, tilePos.y);

            bool pickedMove;
            if(singleClick) {
                // Drag or click a piece.
                pickedMove = ClickPieceMove(fromIndex, toIndex, tilePos);
            } else if(clickRelease && humanState.isHolding){
                // Drop a dragged piece.
                pickedMove = DropPieceMove(fromIndex, toIndex);
            } else {
                pickedMove = false;
            }

            bool shouldMove = false;
            if(pickedMove){
                // Check for promotions so the player can choose.
                if(IsMoveType(humanState.selectedMove.flags, MoveType::Promotion)) {
                    shouldMove = false; // Need to pick first.
                    humanState.promotionMenu = true;
                } else {
                    shouldMove = true;
                }
            }

            return shouldMove;
        }

        bool Game::ClickPieceMove(uint8_t fromIndex, uint8_t toIndex, sf::Vector2i tilePos){
            // If already holding , dont do anything.
            using namespace ChessEngine::BitboardUtil;
            using namespace ChessEngine::MoveGeneration;

            if(humanState.isHolding){
                return false;
            }

            Move move{};
            auto[type, color] = board.GetState().GetPosType(GetSquareIndex(tilePos.x, tilePos.y));
            bool validMove = humanState.activePiece && IndecesToMove(fromIndex, toIndex, humanState.activePieceMoves, move);

            bool pickedMove = false;
            if (validMove) {
                humanState.selectedMove = move;
                shouldMoveAnimation = true;

                humanState.activePiece = false;
                humanState.isHolding = false;
                pickedMove = true;
            } else if (type == ChessEngine::PieceType::None || color != board.GetState().turnOf) {
                // Clicked an invalid move , empty tile.
                humanState.activePiece = false;
                humanState.isHolding = false;
            } else if (color == board.GetState().turnOf) {
                // Clicked another owned piece.
                humanState.holdingSprite = TextureManager::GetPieceSprite(color, type);
                humanState.fromPos = tilePos;

                auto pseudoMoves = GetValidMoves(board.GetState(), board.GetState().turnOf, board.GetUtilities());
                uint8_t index = GetSquareIndex(humanState.fromPos.x, humanState.fromPos.y);
                humanState.activePieceMoves = FromIndexMoves(index, pseudoMoves);

                humanState.activePiece = true;
                humanState.isHolding = true;
            }

            return pickedMove;
        }

        bool Game::DropPieceMove(uint8_t fromIndex, uint8_t toIndex){
            using namespace ChessEngine::BitboardUtil;
            using namespace ChessEngine::MoveGeneration;

            if(!humanState.isHolding){
                return false;
            }

            // Stop holding the piece , even if the move is invalid.
            humanState.isHolding = false;

            Move move{};
            if(IndecesToMove(fromIndex, toIndex, humanState.activePieceMoves, move)) {
                humanState.selectedMove = move;
                shouldMoveAnimation = false; // Disable animations for drag n drop.

                humanState.activePiece = false;
                return true;
            }else{
                return false;
            }
        }

}