#include "retronomicon/graphics/sdl_texture.h"
#include <stdexcept>

namespace retronomicon::graphics::sdl {

    SDLTexture::SDLTexture(SDL_Renderer* renderer, const retronomicon::asset::ImageAsset& image) {
        // Create surface from raw RGBA pixels
        SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(
            (void*)image.getPixels().data(),
            image.getWidth(),
            image.getHeight(),
            32,                                // bits per pixel
            image.getWidth() * 4,              // pitch (bytes per row)
            0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000 // RGBA masks
        );

        if (!surface) {
            throw std::runtime_error("Failed to create SDL_Surface");
        }

        // Create hardware texture from surface
        m_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!m_texture) {
            SDL_FreeSurface(surface);
            throw std::runtime_error("Failed to create SDL_Texture from surface");
        }

        m_width = image.getWidth();
        m_height = image.getHeight();

        SDL_FreeSurface(surface);
    }

    SDLTexture::~SDLTexture() {
        if (m_texture) {
            SDL_DestroyTexture(m_texture);
        }
    }

    int SDLTexture::getWidth() const {
        return m_width;
    }

    int SDLTexture::getHeight() const {
        return m_height;
    }

    void SDLTexture::bind() const {
        // SDL doesn’t use bind/unbind. No-op.
    }

    void SDLTexture::unbind() const {
        // SDL doesn’t use bind/unbind. No-op.
    }

}
