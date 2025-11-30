#pragma once

#include <memory>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "retronomicon/asset/font_asset.h"

namespace retronomicon::sdl::asset {

class SDLFontAsset : public retronomicon::asset::FontAsset {
public:
    SDLFontAsset(const std::string& path, int pointSize)
        : FontAsset(path, pointSize) {}

    SDLFontAsset(const std::string& path,
                 const std::string& name,
                 int pointSize)
        : FontAsset(path, name, pointSize) {}

    virtual ~SDLFontAsset() {
        unload();
    }

    /*************************
     * Backend-specific loader
     *************************/
    bool load(SDL_Renderer* renderer);    // SDL-specific
    bool load() override { return false; } // disable core load()

    void unload() override;
    bool isLoaded() const noexcept override { return m_isLoaded; }

    SDL_Texture* getTextureAtlas() const noexcept { return m_textureAtlas; }
    int getAtlasWidth() const noexcept { return m_atlasWidth; }
    int getAtlasHeight() const noexcept { return m_atlasHeight; }

    std::string to_string() const override;

private:
    bool loadGlyphsAndBuildAtlas(SDL_Renderer* renderer);
    bool renderGlyphToAtlas(SDL_Surface* atlasSurface,
                            char c,
                            int x, int y,
                            TTF_Font* font);

private:
    bool m_isLoaded = false;

    SDL_Texture* m_textureAtlas = nullptr;
    int m_atlasWidth = 0;
    int m_atlasHeight = 0;
};

} // namespace retronomicon::sdl::asset
