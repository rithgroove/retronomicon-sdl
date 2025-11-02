#pragma once

#include "retronomicon/graphics/renderer/i_renderer.h"
#include <SDL.h>
#include <string>

namespace retronomicon::sdl::graphics::renderer {

        using retronomicon::graphics::Texture;
        using retronomicon::math::Vec2;
    /**
     * @brief SDL-based implementation of the IRenderer interface.
     */
    class SDLRenderer : public retronomicon::graphics::renderer::IRenderer {
    public:
        /**
         * @brief Construct an SDLRenderer with given window title, width, and height.
         * @param title The window title.
         * @param width Window width in pixels.
         * @param height Window height in pixels.
         */
        SDLRenderer(const std::string& title, int width, int height);

        /**
         * @brief Destructor shuts down SDL if not already done.
         */
        virtual ~SDLRenderer();

        /// Initialize SDL and create window/renderer.
        void init() override;

        /// Render a test background color.
        void render(std::shared_ptr<Texture> texture,
                        const Vec2& position,
                        const Vec2& scale,
                        float rotation = 0.0f,
                        float alpha = 1.0f) override;

        /// Clean up SDL resources.
        void shutdown() override;

        void show() override;

        void clear() override;
        int getWidth() const override {return m_width;}
        int getHeight() const override {return m_height;}

        SDL_Renderer* getRenderer(){return m_renderer;}
    private:
        std::string m_title;
        int m_width;
        int m_height;

        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
        bool m_initialized = false;
    };
}
