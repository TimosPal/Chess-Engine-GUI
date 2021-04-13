#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <Engine/Utilities/Utilities.h>

namespace ChessFrontend {

    class TextureManager {
    public:
        TextureManager() = delete;

        static void Init(const std::string &spriteFolder);

        static const sf::Sprite& GetPieceSprite(ChessEngine::Color color, ChessEngine::PieceType type){
            return pieceSprites[color][type];
        }

        /* Get a scaling factor to convert "from" to "to". */
        static constexpr float GetScalingFactor(float from, float to){
            return  from / to;
        }

    private:
        // TODO: maybe make more generic if more sprites are needed.
        static sf::Texture piecesSpriteSheet;
        static sf::Sprite pieceSprites[2][6];
    };
}

#endif