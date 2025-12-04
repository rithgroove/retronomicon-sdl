#include "retronomicon/manager/sdl_texture_manager.h"
#include "retronomicon/graphics/sdl_texture.h"
#include "retronomicon/asset/sdl_font_asset.h"
#include <iostream>

namespace retronomicon::sdl::manager {

using retronomicon::sdl::graphics::SDLTexture;

SDLTextureManager::SDLTextureManager(SDL_Renderer* renderer)
    : m_renderer(renderer)
{
}

SDLTextureManager::~SDLTextureManager() = default;

std::shared_ptr<Texture>
SDLTextureManager::createTexture(std::shared_ptr<ImageAsset> imageAsset) {
    return std::make_shared<SDLTexture>(m_renderer, imageAsset);
}

// --------------------------------------------------------------
// NEW: Create SDLTexture from FontAsset pixel atlas
// --------------------------------------------------------------
std::shared_ptr<Texture>
SDLTextureManager::createTexture(std::shared_ptr<FontAsset> fontAsset)
{
    // Ensure backend type
    auto sdlFont = std::dynamic_pointer_cast<retronomicon::sdl::asset::SDLFontAsset>(fontAsset);
    if (!sdlFont) {
        std::cerr << "[SDLTextureManager] ERROR: FontAsset is not SDLFontAsset.\n";
        return nullptr;
    }

    const auto& pixels = sdlFont->getAtlasPixels();
    int width  = sdlFont->getAtlasWidth();
    int height = sdlFont->getAtlasHeight();

    if (pixels.empty() || width == 0 || height == 0) {
        std::cerr << "[SDLTextureManager] ERROR: SDLFontAsset has no atlas pixels.\n";
        return nullptr;
    }

    // Create the GPU texture
    SDL_Texture* texture = SDL_CreateTexture(
        m_renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STATIC,
        width,
        height
    );

    if (!texture) {
        std::cerr << "[SDLTextureManager] ERROR: Failed to create SDL_Texture: "
                  << SDL_GetError() << "\n";
        return nullptr;
    }

    // Upload pixel buffer into SDL_Texture
    if (SDL_UpdateTexture(texture, nullptr, pixels.data(), width * 4) != 0) {
        std::cerr << "[SDLTextureManager] ERROR: Failed to upload texture pixels: "
                  << SDL_GetError() << "\n";
        SDL_DestroyTexture(texture);
        return nullptr;
    }

    // Wrap the backend texture
    return std::make_shared<SDLTexture>(m_renderer, texture);
}

} // namespace retronomicon::sdl::manager
