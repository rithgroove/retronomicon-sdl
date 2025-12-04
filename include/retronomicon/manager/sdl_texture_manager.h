#pragma once

#include <memory>
#include <SDL.h>

#include "retronomicon/graphics/texture.h"
#include "retronomicon/manager/texture_manager.h"
#include "retronomicon/asset/image_asset.h"
#include "retronomicon/asset/font_asset.h"

namespace retronomicon::sdl::manager {

    using retronomicon::manager::TextureManager;
    using retronomicon::graphics::Texture;
    using retronomicon::asset::ImageAsset;
    using retronomicon::asset::FontAsset;

    class SDLTextureManager : public TextureManager {
    public:
        SDLTextureManager(SDL_Renderer* renderer);
        ~SDLTextureManager();

        std::shared_ptr<Texture> createTexture(std::shared_ptr<ImageAsset> imageAsset) override;

        // NEW: create texture from font atlas
        std::shared_ptr<Texture> createTexture(std::shared_ptr<FontAsset> fontAsset) override;

    private:
        SDL_Renderer* m_renderer; // not owned
    };

} // namespace retronomicon::sdl::manager
