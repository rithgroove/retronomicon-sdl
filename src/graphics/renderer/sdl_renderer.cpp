#include "retronomicon/graphics/renderer/sdl_renderer.h"
#include <stdexcept>
#include <iostream>

namespace retronomicon::sdl::graphics::renderer {

    SDLRenderer::SDLRenderer(const std::string& title, int width, int height)
        : m_title(title), m_width(width), m_height(height) {}

    SDLRenderer::~SDLRenderer() {
        shutdown();
    }

    void SDLRenderer::init() {
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
        }

        m_window = SDL_CreateWindow(
            m_title.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            m_width,
            m_height,
            SDL_WINDOW_SHOWN
        );

        if (!m_window) {
            throw std::runtime_error("Failed to create SDL window: " + std::string(SDL_GetError()));
        }

        m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
        if (!m_renderer) {
            throw std::runtime_error("Failed to create SDL renderer: " + std::string(SDL_GetError()));
        }

        m_initialized = true;
        std::cout << "SDLRenderer initialized with window: " << m_title << std::endl;
    }

    void SDLRenderer::render() {
        if (!m_initialized) return;

        // Clear screen with blue
        SDL_SetRenderDrawColor(m_renderer, 0, 100, 255, 255);
        SDL_RenderClear(m_renderer);

        // Present to screen
        SDL_RenderPresent(m_renderer);
    }

    void SDLRenderer::shutdown() {
        if (m_renderer) {
            SDL_DestroyRenderer(m_renderer);
            m_renderer = nullptr;
        }
        if (m_window) {
            SDL_DestroyWindow(m_window);
            m_window = nullptr;
        }
        if (m_initialized) {
            SDL_Quit();
            m_initialized = false;
        }
    }

}
