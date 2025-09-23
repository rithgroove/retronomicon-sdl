#pragma once

#include <SDL.h>
#include <stdexcept>
#include "retronomicon-core/graphics/i_window.h"

namespace retronomicon::sdl::graphics {

/**
 * @brief SDL implementation of the IWindow interface.
 *
 * Uses SDL2 to create and manage a game/application window. 
 * This class provides a bridge between the engine's abstract 
 * IWindow interface and SDL's platform-specific implementation.
 */
class SDLWindow : public retronomicon::graphics::IWindow {
        public:
            /**
             * @brief Construct a new SDLWindow object.
             *
             * @param title   Title of the window.
             * @param width   Initial width in pixels.
             * @param height  Initial height in pixels.
             *
             * @throws std::runtime_error if SDL window creation fails.
             */
            SDLWindow(const std::string& title, int width, int height);

            /**
             * @brief Destroy the SDLWindow object.
             *
             * Cleans up SDL resources.
             */
            ~SDLWindow() override;

            // ------------------------------------------------------------------------
            // IWindow overrides
            // ------------------------------------------------------------------------

            bool isFullscreen() const override;
            void toggleFullscreen() override;
            void handleResize(int newWidth, int newHeight) override;

            // ------------------------------------------------------------------------
            // Extra utilities
            // ------------------------------------------------------------------------

            /**
             * @brief Clear the window with a color.
             *
             * @param r Red (0–255).
             * @param g Green (0–255).
             * @param b Blue (0–255).
             * @param a Alpha (0–255).
             */
            void clear(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);

            /**
             * @brief Present the back buffer to the screen.
             */
            void present();

            /**
             * @brief Get raw SDL_Window pointer (for integration with other APIs).
             */
            SDL_Window* getSDLWindow() const { return m_window; }

            /**
             * @brief Get raw SDL_Renderer pointer (for rendering directly with SDL).
             */
            SDL_Renderer* getSDLRenderer() const { return m_renderer; }

        private:
            SDL_Window* m_window = nullptr;    ///< Underlying SDL window.
            SDL_Renderer* m_renderer = nullptr;///< SDL renderer tied to the window.
    };

} // namespace retronomicon::sdl::graphics
