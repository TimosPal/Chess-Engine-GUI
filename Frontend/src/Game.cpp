#include "Game.h"

#include <SFML/Window/Event.hpp>

#include <Engine/MoveGeneration/MoveGeneration.h>
#include <Engine/MoveGeneration/Draw.h>

#include "./RenderingUtil.h"
#include "ResourceManager.h"

// TODO : better bitboard array representation , monochrome?

namespace ChessFrontend {

        Game::Game(ChessEngine::BoardState state, const Options& options)
                : window(sf::VideoMode(
                         options.windowSettings.width,
                         options.windowSettings.height),
                         options.windowSettings.title, sf::Style::None),
                         board(state), options(options),
                         humanState(options.startingView)
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
            RenderingUtil::DrawCoordinates(window, humanState.viewSide);

            auto bitboardW = board.GetState().pieceBoards[ChessEngine::White][ChessEngine::PieceType::Pawn];
            auto bitboardB = board.GetState().pieceBoards[ChessEngine::Black][ChessEngine::PieceType::Pawn];
            auto bitboard = bitboardW;
            RenderingUtil::Draw_Bitboard(window, bitboard, humanState.viewSide);
            RenderingUtil::DrawPieces(window, board.GetOccupancies(), GetIgnoreList(), humanState.viewSide);

            if(humanState.promotionMenu){
                // Create vector for the origin of the window.
                auto [x,y] = ChessEngine::BitboardUtil::GetCoordinates(humanState.selectedMove.toSquareIndex);
                sf::Vector2i windowOrigin(x,y);

                RenderingUtil::DrawPromotionMenu(window, windowOrigin, humanState.viewSide);
            }

            // IsDraw available moves and the holding piece.
            if(humanState.activePiece) {
                RenderingUtil::DrawActivePieceMoves(window, humanState.activePieceMoves, humanState.viewSide);
                if (humanState.isHolding)
                    RenderingUtil::DrawHoldingPiece(window, humanState.holdingSprite);
            }

            // Handle the move animations.
            if(playMoveAnimation) {
                float lerpTime = elapsedAnimTime / options.secPerMove;
                ChessEngine::Color turnOf = board.GetState().turnOf;
                playMoveAnimation = RenderingUtil::PlayMoveAnimation(window, humanState.selectedMove, turnOf, humanState.viewSide, lerpTime);

                if(playMoveAnimation)
                    elapsedAnimTime += dt.asSeconds();
                else {
                    elapsedAnimTime = 0;
                    SwapSides(); // Swap sides when animation is over.
                }
            }

            window.display();
        }

        void Game::SwapSides(){
            if(options.sideSwap){
                humanState.viewSide = ChessEngine::InvertColor(humanState.viewSide);
            }
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
                auto[toX, toY] = ChessEngine::BitboardUtil::GetCoordinates(humanState.selectedMove.toSquareIndex);
                ignoreList.emplace_back(toX, toY);
                if (IsMoveType(humanState.selectedMove.flags,
                               (MoveType) (MoveType::QueenSideCastling | MoveType::KingSideCastling))) {
                    // If castling move , exclude rook move during animation.
                    uint8_t rookNewIndex = IsMoveType(humanState.selectedMove.flags, MoveType::QueenSideCastling) ?
                                           humanState.selectedMove.toSquareIndex + 1 :
                                           humanState.selectedMove.toSquareIndex - 1;

                    auto[toX, toY] = ChessEngine::BitboardUtil::GetCoordinates(rookNewIndex);
                    ignoreList.emplace_back(toX, toY);
                }
            }

            return ignoreList;
        }

        void Game::Update(){
            auto& gameState = board.GetState().gameState;
            if(gameState == ChessEngine::GameState::Playing) {
                PlayMove();
            }else{
                std::cout << ChessEngine::GameStateToString(gameState) << std::endl;
                window.close(); // TODO: temporary solution.
            }
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

            auto moves = GetValidMoves(board.GetState(), board.GetState().turnOf, board.GetOccupancies());

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

            MakeMove(mv, board.GetState().turnOf, board.GetState(), board.GetOccupancies());

            humanState.selectedMove = mv;
            playMoveAnimation = true;

            return true; // Plays move instantly.
        }

        bool Game::HumanTurn(){
            using namespace ChessEngine::BitboardUtil;
            using namespace ChessEngine::MoveGeneration;

            sf::Vector2i tileSize(window.getSize().x / 8, window.getSize().y / 8);
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);

            // Color side view.
            ChessEngine::Color sideView = humanState.viewSide;
            int tilePosX = mousePos.x / tileSize.x;
            int tilePosY = mousePos.y / tileSize.y;
            int posX = (sideView == ChessEngine::Color::White ? tilePosX : 7 - tilePosX);
            int posY = (sideView == ChessEngine::Color::White ? 7 - tilePosY : tilePosY);
            sf::Vector2i relativeTilePos(posX, posY);

            bool shouldMove;
            if(!humanState.promotionMenu) {
                shouldMove = PickMove(relativeTilePos);
            }else{
                shouldMove = PickPromotion(board.GetState().turnOf, relativeTilePos);
            }

            if(shouldMove){
                MakeMove(humanState.selectedMove, board.GetState().turnOf, board.GetState(), board.GetOccupancies());
                playMoveAnimation = shouldMoveAnimation;

                // If we got no animation swap instantly.
                if(!shouldMoveAnimation)
                    SwapSides();

                // TODO: maybbe too slow.
                auto moves = GetValidMoves(board.GetState(), board.GetState().turnOf, board.GetOccupancies());
                if(Draw::IsDraw(board, moves)){
                    board.GetState().gameState = ChessEngine::GameState::Draw;
                }
                if(Draw::IsCheckmate(board, moves)){
                    board.GetState().gameState = ChessEngine::GameState::Win;
                }

                return true;
            }else{
                return false;
            }
        }

        bool Game::PickPromotion(ChessEngine::Color color, sf::Vector2i tilePos){
            if(!singleClick){
                return false;
            }

            int queenSelection, knightSelection, rookSelection, bishopSelection;
            if(color == ChessEngine::Color::White){
                queenSelection = 7;
                knightSelection = 6;
                rookSelection = 5;
                bishopSelection = 4;
            }else{
                queenSelection = 0;
                knightSelection = 1;
                rookSelection = 2;
                bishopSelection = 3;
            }

            // We need to find the appropriate menu bounds based on the color.
            bool pickedPromotion = true;
            auto [x,y] = ChessEngine::BitboardUtil::GetCoordinates(humanState.selectedMove.toSquareIndex);
            bool correctColumn = tilePos.x == x;
            if(tilePos.y == queenSelection && correctColumn){
                humanState.selectedMove.promotionType = ChessEngine::PieceType::Queen;
            }else if(tilePos.y == knightSelection && correctColumn){
                humanState.selectedMove.promotionType = ChessEngine::PieceType::Knight;
            }else if(tilePos.y == rookSelection && correctColumn){
                humanState.selectedMove.promotionType = ChessEngine::PieceType::Rook;
            }else if(tilePos.y == bishopSelection && correctColumn){
                humanState.selectedMove.promotionType = ChessEngine::PieceType::Bishop;
            }else{
                pickedPromotion = false; // Out of bounds click.
            }

            humanState.promotionMenu = false; // Always turns off after a click.
            return pickedPromotion;
        }

        bool Game::PickMove(sf::Vector2i tilePos){
            using namespace ChessEngine::BitboardUtil;
            using namespace ChessEngine::MoveGeneration;

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

            auto[type, color] = board.GetState().GetPosType(GetSquareIndex(tilePos.x, tilePos.y));

            // Check if a valid destination.
            bool validMove = false;
            Move move{};
            if(humanState.activePiece){
                validMove = IndecesToMove(fromIndex, toIndex, humanState.activePieceMoves, move);
            }

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
                humanState.holdingSprite = ResourceManager::GetPieceSprite(color, type);
                humanState.fromPos = tilePos;

                auto moves = GetValidMoves(board.GetState(), board.GetState().turnOf, board.GetOccupancies());
                uint8_t index = GetSquareIndex(humanState.fromPos.x, humanState.fromPos.y);
                humanState.activePieceMoves = FromIndexMoves(index, moves);

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