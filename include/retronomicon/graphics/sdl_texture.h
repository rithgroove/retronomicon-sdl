#pragma once

#include <memory>
#include <SDL.h>
#include "retronomicon/graphics/texture.h"
#include "retronomicon/asset/image_asset.h"

namespace retronomicon::sdl::graphics {
    using retronomicon::graphics::Texture;

    class SDLTexture : public Texture {
    public:
        SDLTexture(SDL_Renderer* renderer, const retronomicon::asset::ImageAsset& image);
        ~SDLTexture();

        int getWidth() const override;
        int getHeight() const override;
        void bind() const override;   // No-op
        void unbind() const override; // No-op

        SDL_Texture* getNativeTexture() const { return m_texture; }

    private:
        SDL_Texture* m_texture = nullptr;
        int m_width = 0;
        int m_height = 0;
    };
}
