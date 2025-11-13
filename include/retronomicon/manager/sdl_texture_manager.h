#pragma once

#include <memory>
#include <SDL.h>
#include "retronomicon/graphics/texture.h"
#include "retronomicon/manager/texture_manager.h"
#include "retronomicon/asset/image_asset.h"  

namespace retronomicon::sdl::manager{
	using retronomicon::manager::TextureManager;
    using retronomicon::graphics::Texture;
    using retronomicon::asset::ImageAsset;
	class SDLTextureManager:public TextureManager{
		public:
			SDLTextureManager(SDL_Renderer* renderer);
			~SDLTextureManager();
			std::shared_ptr<Texture>  createTexture(std::shared_ptr<ImageAsset> ImageAsset) override;
		private:
			SDL_Renderer* m_renderer;
	};

}