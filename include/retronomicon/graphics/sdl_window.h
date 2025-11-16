#pragma once

#include <SDL.h>
#include <stdexcept>
#include "retronomicon/graphics/i_window.h"

namespace retronomicon::sdl::graphics {

class SDLWindow : public retronomicon::graphics::IWindow {
public:
    SDLWindow(const std::string& title, int width, int height);
    ~SDLWindow() override;

    bool isFullscreen() const override;
    void toggleFullscreen() override;
    void handleResize(int newWidth, int newHeight) override;

    void clear(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);
    void present();

    SDL_Window* getSDLWindow() const { return m_window; }
    SDL_Renderer* getSDLRenderer() const { return m_renderer; }

private:
    SDL_Window*   m_window   = nullptr;
    SDL_Renderer* m_renderer = nullptr;
};

} // namespace retronomicon::sdl::graphics
