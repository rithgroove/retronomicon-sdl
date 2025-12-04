#include "retronomicon/manager/sdl_texture_manager.h"
#include "retronomicon/graphics/sdl_texture.h"
#include "retronomicon/asset/sdl_font_asset.h"

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

// ---------------------------------------------------------------------------
// NEW: Create an SDLTexture from a FontAsset atlas
// ---------------------------------------------------------------------------
std::shared_ptr<Texture>
SDLTextureManager::createTexture(std::shared_ptr<FontAsset> fontAsset)
{
    // Downcast into backend implementation
    auto sdlFont = std::dynamic_pointer_cast<retronomicon::sdl::asset::SDLFontAsset>(fontAsset);
    if (!sdlFont) {
        std::cerr << "[SDLTextureManager] ERROR: FontAsset is not SDLFontAsset.\n";
        return nullptr;
    }

    SDL_Surface* atlasSurface = sdlFont->getAtlasSurface();
    if (!atlasSurface) {
        std::cerr << "[SDLTextureManager] ERROR: SDLFontAsset has no atlas surface.\n";
        return nullptr;
    }

    // Create SDL_Texture from the atlas surface
    SDL_Texture* tex = SDL_CreateTextureFromSurface(m_renderer, atlasSurface);
    if (!tex) {
        std::cerr << "[SDLTextureManager] ERROR: Failed to create SDL_Texture: "
                  << SDL_GetError() << "\n";
        return nullptr;
    }

    // Wrap SDL_Texture inside SDLTexture (your backend texture class)
    return std::make_shared<SDLTexture>(m_renderer, tex);
}

} // namespace retronomicon::sdl::manager
