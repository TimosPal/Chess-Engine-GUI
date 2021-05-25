#include "ResourceManager.h"

#include <string>

#include <cassert>

// Paths are from within the sprite folder.
#define PIECES_SPRITE_PATH "pieces.png"
#define FONT_PATH "ariblk.ttf"

namespace ChessFrontend {

        sf::Texture ResourceManager::piecesSpriteSheet{};
        sf::Sprite ResourceManager::pieceSprites[2][6]{};

        sf::Font ResourceManager::defaultFont{};

        void ResourceManager::Init(const std::string &spriteFolder, const std::string& fontsFolder) {
            InitTextures(spriteFolder);
            InitFonts(fontsFolder);
        }

        void ResourceManager::InitTextures(const std::string &spriteFolder){
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

        void ResourceManager::InitFonts(const std::string& fontsFolder){
            bool loadedFile = defaultFont.loadFromFile(fontsFolder + '/' + FONT_PATH);
            assert(loadedFile);
        }

}