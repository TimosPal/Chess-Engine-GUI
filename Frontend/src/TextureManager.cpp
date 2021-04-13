#include "TextureManager.h"

#include <string>

#include <cassert>

// Paths are from within the sprite folder.
#define PIECES_SPRITE_PATH "pieces.png"

namespace ChessFrontend {

        sf::Texture TextureManager::piecesSpriteSheet{};
        sf::Sprite TextureManager::pieceSprites[2][6]{};

        void TextureManager::Init(const std::string &spriteFolder) {
            bool loadedFile = piecesSpriteSheet.loadFromFile(spriteFolder + '/' + PIECES_SPRITE_PATH);
            assert(loadedFile);

            // NOTE: white pieces are first.
            // Image is 2 x 6 (in tiles)
            // the order of the sprites is the same as the order of the PieceType and Color enums.
            // So the indexing can be done with [Color][PieceType].

            int pieceSpriteSize = (int)piecesSpriteSheet.getSize().x / 6;
            for (int i = 0; i < 2; i++) {
                for (int j = 0; j < 6; j++) {
                    sf::Sprite currPieceSprite;
                    currPieceSprite.setTexture(piecesSpriteSheet);
                    auto subTextureRect = sf::IntRect(j * pieceSpriteSize, i * pieceSpriteSize, pieceSpriteSize,
                                                      pieceSpriteSize);
                    currPieceSprite.setTextureRect(subTextureRect);

                    pieceSprites[i][j] = currPieceSprite;
                }
            }

        }

}