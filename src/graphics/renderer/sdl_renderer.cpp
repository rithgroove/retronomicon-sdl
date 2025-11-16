#include "retronomicon/graphics/renderer/sdl_renderer.h"
#include "retronomicon/graphics/sdl_texture.h"
#include <iostream>

namespace retronomicon::sdl::graphics::renderer {

SDLRenderer::SDLRenderer(SDL_Window* window,
                         SDL_Renderer* renderer,
                         int width,
                         int height)
    : m_window(window),
      m_renderer(renderer),
      m_width(width),
      m_height(height)
{
}

SDLRenderer::~SDLRenderer() {
    shutdown();
}

void SDLRenderer::init() {
    if (!m_window || !m_renderer) {
        throw std::runtime_error("SDLRenderer::init() — window or renderer is null!");
    }

    SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);

    m_initialized = true;
    std::cout << "SDLRenderer initialized.\n";
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
                         float alpha)
{
    if (!texture) return;

    Rect target{
        position.x, position.y,
        texture->getWidth() * scale.x,
        texture->getHeight() * scale.y
    };

    Rect source{
        0, 0,
        static_cast<float>(texture->getWidth()),
        static_cast<float>(texture->getHeight())
    };

    renderQuad(texture, target, source, rotation, alpha, Color::White());
}

void SDLRenderer::renderQuad(std::shared_ptr<Texture> texture,
                             const Rect& target,
                             const Rect& source,
                             float rotation,
                             float alpha,
                             const Color& color)
{
    auto sdlTex = std::dynamic_pointer_cast<retronomicon::sdl::graphics::SDLTexture>(texture);
    if (!sdlTex) {
        std::cerr << "Texture isn't an SDLTexture!\n";
        return;
    }

    SDL_Texture* rawTex = sdlTex->getNativeTexture();
    if (!rawTex) return;

    SDLColor sdlColor(color);
    sdlColor.applyToTexture(rawTex);

    SDL_Rect srcRect{
        (int)source.getX(),
        (int)source.getY(),
        (int)source.getWidth(),
        (int)source.getHeight()
    };

    float anchorX = target.getWidth()  * target.getAnchor().getX();
    float anchorY = target.getHeight() * target.getAnchor().getY();

    SDL_FRect dstRect{
        target.getX() - anchorX,
        target.getY() - anchorY,
        target.getWidth(),
        target.getHeight()
    };

    SDL_FPoint center{ anchorX, anchorY };

    SDL_SetTextureAlphaMod(rawTex, static_cast<Uint8>(alpha * 255));

    SDL_RenderCopyExF(
        m_renderer,
        rawTex,
        &srcRect,
        &dstRect,
        rotation,
        &center,
        SDL_FLIP_NONE
    );
}

void SDLRenderer::show() {
    if (!m_initialized) return;
    SDL_RenderPresent(m_renderer);
}

void SDLRenderer::shutdown() {
    m_initialized = false;

    // ❗ DO NOT destroy renderer or window here
    // SDLWindow owns destruction
}

bool SDLRenderer::shouldClose() const {
    return false; // SDL doesn't track this like GLFW; engine loop handles SDL_QUIT
}

} // namespace retronomicon::sdl::graphics::renderer
