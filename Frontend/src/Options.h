#ifndef OPTIONS_H
#define OPTIONS_H

#include "WindowSettings.h"

namespace ChessFrontend {
    /* Options used inside the game class */
    struct Options {
        const bool whiteAI;
        const bool blackAI;
        const bool sideSwap;

        ChessEngine::Color startingView;

        const float secPerMove;

        const WindowSettings windowSettings;

        Options(bool whiteAI, bool blackAI, float secPerMove, bool sideSwap, ChessEngine::Color startingView, const WindowSettings& windowSettings)
        : whiteAI(whiteAI), blackAI(blackAI), secPerMove(secPerMove), windowSettings(windowSettings), startingView(startingView), sideSwap(sideSwap)
        {}
    };

}

#endif