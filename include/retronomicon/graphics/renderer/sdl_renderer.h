#pragma once

#include "retronomicon/graphics/renderer/i_renderer.h"
#include "retronomicon/graphics/sdl_color.h"
#include <SDL.h>
#include <string>

namespace retronomicon::sdl::graphics::renderer {

using retronomicon::graphics::Texture;
using retronomicon::graphics::Color;
using retronomicon::math::Vec2;
using retronomicon::math::Rect;
using retronomicon::sdl::graphics::SDLColor;

/**
 * @brief SDL-based implementation of the IRenderer interface.
 */
class SDLRenderer : public retronomicon::graphics::renderer::IRenderer {
public:
    SDLRenderer(const std::string& title, int width, int height);
    ~SDLRenderer() override;

    void init() override;
    void clear() override;
    void show() override;
    void shutdown() override;

    void render(std::shared_ptr<Texture> texture,
                const Vec2& position,
                const Vec2& scale,
                float rotation = 0.0f,
                float alpha = 1.0f) override;

    void renderQuad(std::shared_ptr<Texture> texture,
                    const Rect& target,
                    const Rect& source,
                    float rotation = 0.0f,
                    float alpha = 1.0f,
                    const Color& color = Color::White()) override;

    int getWidth() const override { return m_width; }
    int getHeight() const override { return m_height; }

    SDL_Renderer* getRenderer() { return m_renderer; }

private:
    std::string m_title;
    int m_width;
    int m_height;

    SDL_Window* m_window = nullptr;
    SDL_Renderer* m_renderer = nullptr;
    bool m_initialized = false;
};

} // namespace retronomicon::sdl::graphics::renderer
