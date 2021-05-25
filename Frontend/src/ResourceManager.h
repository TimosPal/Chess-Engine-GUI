#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Font.hpp>

#include <Engine/Utilities/Utilities.h>

namespace ChessFrontend {

    /* NOTE: Since we dont require many different resources a single
     * class was made .Also this class isn't abstracted properly
     * since we dont require anything more advanced for this project. */

    class ResourceManager {
    public:
        ResourceManager() = delete;

        static void Init(const std::string &spriteFolder, const std::string& fontsFolder);

        static const sf::Sprite& GetPieceSprite(ChessEngine::Color color, ChessEngine::PieceType type){
            return pieceSprites[color][type];
        }

        /* Get a scaling factor to convert "from" to "to". */
        static constexpr float GetScalingFactor(float from, float to){
            return  from / to;
        }

        static const sf::Font& GetFont(){
            return defaultFont;
        }

    private:
        // TODO: maybe make more generic if more sprites are needed.
        static sf::Texture piecesSpriteSheet;
        static sf::Sprite pieceSprites[2][6];

        static sf::Font defaultFont;

        static void InitTextures(const std::string &spriteFolder);
        static void InitFonts(const std::string &fontsFolder);
    };
}

#endif