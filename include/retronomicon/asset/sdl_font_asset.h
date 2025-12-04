#pragma once

#include <vector>
#include <string>
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

    ~SDLFontAsset() override { unload(); }

    // ------------------------------------------------------------
    // Core-compatible load/unload API
    // ------------------------------------------------------------
    bool load() override;         // now backend-specific, no renderer needed
    void unload() override;
    bool isLoaded() const noexcept override { return m_isLoaded; }

    // Pixel atlas (used by SDLTextureManager)
    const std::vector<uint8_t>& getAtlasPixels() const noexcept { return m_pixels; }
    int getAtlasWidth() const noexcept { return m_atlasWidth; }
    int getAtlasHeight() const noexcept { return m_atlasHeight; }

    std::string to_string() const override;

private:
    bool loadGlyphs();  // extract glyph metrics + bitmap surfaces
    bool buildAtlas();  // pack bitmaps into m_pixels

private:
    bool m_isLoaded = false;

    // Atlas image
    std::vector<uint8_t> m_pixels; // RGBA
    int m_atlasWidth = 0;
    int m_atlasHeight = 0;

    // Temporary glyph surfaces before packing
    struct GlyphSurface {
        SDL_Surface* surf;
        int width;
        int height;
        int bearingX;
        int bearingY;
        int advance;
    };
    std::unordered_map<char, GlyphSurface> m_surfaces;
};

} // namespace retronomicon::sdl::asset
