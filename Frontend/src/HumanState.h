#ifndef HUMANSTATE_H
#define HUMANSTATE_H

#include <list>
#include <Engine/MoveGeneration/Move.h>

namespace ChessFrontend {

    struct HumanState {
        bool activePiece; // there is an active piece.
        bool isHolding; // is draging to move a piece.
        bool promotionMenu; // if the promo menu is open.

        sf::Sprite holdingSprite; // Piece sprite being draged.
        sf::Vector2i fromPos; // origin of selected move.

        ChessEngine::MoveGeneration::Move selectedMove; // The move that was chosen to be played.
        std::list<ChessEngine::MoveGeneration::Move> activePieceMoves; // Available moves from active piece.

        HumanState(){
            activePiece = false;
            isHolding = false;
            promotionMenu = false;

            selectedMove.flags = ChessEngine::MoveGeneration::MoveType::None;
        }
    };

}

#endif