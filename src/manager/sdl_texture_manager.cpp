#include "retronomicon/manager/sdl_texture_manager.h"
#include "retronomicon/graphics/sdl_texture.h"
namespace retronomicon::sdl::manager{
	using retronomicon::sdl::graphics::OpenGLTexture;
	SDLTextureManager::SDLTextureManager(){

	}

	// OpenGLTextureManager::~OpenGLTextureManager()= default;

	std::shared_ptr<Texture>  OpenGLTextureManager::createTexture(std::shared_ptr<ImageAsset> imageAsset){
		return std::make_shared<SDLTexture>(imageAsset);
	}
}