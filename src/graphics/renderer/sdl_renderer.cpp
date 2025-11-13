#include "retronomicon/graphics/renderer/sdl_renderer.h"
#include "retronomicon/graphics/sdl_texture.h" // your SDLTexture class
#include <stdexcept>
#include <iostream>

namespace retronomicon::sdl::graphics::renderer {

SDLRenderer::SDLRenderer(const std::string& title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {}

SDLRenderer::~SDLRenderer() {
    shutdown();
}

void SDLRenderer::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));

    m_window = SDL_CreateWindow(
        m_title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_width,
        m_height,
        SDL_WINDOW_SHOWN
    );
    if (!m_window)
        throw std::runtime_error("Failed to create SDL window: " + std::string(SDL_GetError()));

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_renderer)
        throw std::runtime_error("Failed to create SDL renderer: " + std::string(SDL_GetError()));

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    m_initialized = true;
    std::cout << "SDLRenderer initialized with window: " << m_title << std::endl;
}

void SDLRenderer::clear() {
    if (!m_initialized) return;
    SDL_SetRenderDrawColor(m_renderer, 25, 25, 50, 255);
    SDL_RenderClear(m_renderer);
}

void SDLRenderer::render(std::shared_ptr<Texture> texture,
                         const Vec2& position,
                         const Vec2& scale,
                         float rotation,
                         float alpha) {
    if (!m_initialized || !texture) return;

    Rect target{position.x, position.y,
                texture->getWidth() * scale.x,
                texture->getHeight() * scale.y};
    Rect source{0, 0,
                float(texture->getWidth()),
                float(texture->getHeight())};

    renderQuad(texture, target, source, rotation, alpha, Color::White());
}

void SDLRenderer::renderQuad(std::shared_ptr<Texture> texture,
                             const Rect& target,
                             const Rect& source,
                             float rotation,
                             float alpha,
                             const Color& color) {
    if (!m_initialized || !texture) return;

    // Dynamic cast to SDL-specific texture type
    auto sdlTex = std::dynamic_pointer_cast<retronomicon::sdl::graphics::SDLTexture>(texture);
    if (!sdlTex) {
        std::cerr << "RenderQuad: Texture is not an SDLTexture instance!" << std::endl;
        return;
    }

    SDL_Texture* rawTex = sdlTex->getNativeTexture();
    if (!rawTex) return;

    SDLColor sdlColor(color);
    sdlColor.applyToTexture(rawTex);

    SDL_Rect srcRect{static_cast<int>(source.getX()),
                     static_cast<int>(source.getY()),
                     static_cast<int>(source.getWidth()),
                     static_cast<int>(source.getHeight())};

    float anchorModifierX = target.getWidth()*target.getAnchor().getX();
    float anchorModifierY = target.getHeight()*target.getAnchor().getY();
    float centerX = target.getX();
    float centerY = target.getY();
    float x = target.getX()-anchorModifierX;
    float y = target.getY()-anchorModifierY;

    SDL_FRect dstRect{x, y, target.getWidth(), target.getHeight()};

    SDL_FPoint center{
        anchorModifierX,
        anchorModifierY
    };

    // Apply alpha
    Uint8 alphaByte = static_cast<Uint8>(alpha * 255);
    SDL_SetTextureAlphaMod(rawTex, alphaByte);

    // Draw rotated quad
    SDL_RenderCopyExF(m_renderer, rawTex, &srcRect, &dstRect,
                      rotation, &center, SDL_FLIP_NONE);
}

void SDLRenderer::show() {
    if (!m_initialized) return;
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

} // namespace retronomicon::sdl::graphics::renderer
