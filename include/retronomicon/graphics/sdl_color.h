#pragma once
#include "retronomicon/graphics/color.h"
#include <SDL2/SDL.h>

namespace retronomicon::sdl::graphics {

    class SDLColor : public retronomicon::graphics::Color {
        public:
            using retronomicon::graphics::Color::Color; // Inherit constructors

            SDL_Color toNative() const noexcept {
                SDL_Color c;
                c.r = static_cast<Uint8>(m_r * 255.0f);
                c.g = static_cast<Uint8>(m_g * 255.0f);
                c.b = static_cast<Uint8>(m_b * 255.0f);
                c.a = static_cast<Uint8>(m_a * 255.0f);
                return c;
            }

            void applyToTexture(SDL_Texture* tex) const noexcept {
                SDL_SetTextureColorMod(tex, static_cast<Uint8>(m_r * 255.0f),
                                            static_cast<Uint8>(m_g * 255.0f),
                                            static_cast<Uint8>(m_b * 255.0f));
                SDL_SetTextureAlphaMod(tex, static_cast<Uint8>(m_a * 255.0f));
            }
    };

} // namespace retronomicon::sdl::graphics
