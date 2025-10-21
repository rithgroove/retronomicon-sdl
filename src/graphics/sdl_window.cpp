#include "retronomicon/graphics/sdl_window.h"

namespace retronomicon::sdl::graphics {

    SDLWindow::SDLWindow(const std::string& title, int width, int height)
        : IWindow(title, width, height) 
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
        }

        m_window = SDL_CreateWindow(
            m_title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_width,
            m_height,
            SDL_WINDOW_RESIZABLE
        );

        if (!m_window) {
            throw std::runtime_error("Failed to create SDL window: " + std::string(SDL_GetError()));
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if (!m_renderer) {
            throw std::runtime_error("Failed to create SDL renderer: " + std::string(SDL_GetError()));
        }
    }

    SDLWindow::~SDLWindow() {
        if (m_renderer) SDL_DestroyRenderer(m_renderer);
        if (m_window) SDL_DestroyWindow(m_window);
        SDL_Quit();
    }

    bool SDLWindow::isFullscreen() const {
        Uint32 flags = SDL_GetWindowFlags(m_window);
        return (flags & SDL_WINDOW_FULLSCREEN) || (flags & SDL_WINDOW_FULLSCREEN_DESKTOP);
    }

    void SDLWindow::toggleFullscreen() {
        if (isFullscreen()) {
            SDL_SetWindowFullscreen(m_window, 0); // windowed
        } else {
            SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
        }
    }

    void SDLWindow::handleResize(int newWidth, int newHeight) {
        IWindow::handleResize(newWidth, newHeight);
        SDL_SetWindowSize(m_window, newWidth, newHeight);
    }

    void SDLWindow::clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
        SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
        SDL_RenderClear(m_renderer);
    }

    void SDLWindow::present() {
        SDL_RenderPresent(m_renderer);
    }

} // namespace retronomicon::sdl::graphics
