#include "retronomicon/manager/sdl_texture_manager.h"
#include "retronomicon/graphics/sdl_texture.h"
namespace retronomicon::sdl::manager{
	using retronomicon::sdl::graphics::SDLTexture;
	SDLTextureManager::SDLTextureManager(SDL_Renderer* renderer): 
		m_renderer(renderer)
	{

	}

	SDLTextureManager::~SDLTextureManager()= default;

	std::shared_ptr<Texture>  SDLTextureManager::createTexture(std::shared_ptr<ImageAsset> imageAsset){
		return std::make_shared<SDLTexture>(m_renderer,imageAsset);
	}
}