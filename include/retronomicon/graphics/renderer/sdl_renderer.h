#pragma once

#include "retronomicon/graphics/renderer/i_renderer.h"
#include <SDL2/SDL.h>
#include <string>

namespace retronomicon::sdl {

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
        void render() override;

        /// Clean up SDL resources.
        void shutdown() override;

    private:
        std::string m_title;
        int m_width;
        int m_height;

        SDL_Window* m_window = nullptr;
        SDL_Renderer* m_renderer = nullptr;
        bool m_initialized = false;
    };
}
