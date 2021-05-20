#ifndef WINDOW_SETTINGS_H
#define WINDOW_SETTINGS_H

#include <string>

namespace ChessFrontend {
    struct WindowSettings {
        const int height;
        const int width;
        const int frameLimit;

        const std::string title;

        WindowSettings(int height, int width, int frameLimit , const std::string& title)
        : height(height) , width(width), frameLimit(frameLimit), title(title)
        {}
    };
}

#endif
