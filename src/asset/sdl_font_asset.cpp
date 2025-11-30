#include "retronomicon/asset/sdl_font_asset.h"
#include <iostream>

namespace retronomicon::sdl::asset {

std::string SDLFontAsset::to_string() const {
    return "SDLFontAsset(name=" + m_name +
           ", path=" + m_path +
           ", size=" + std::to_string(m_pointSize) + ")";
}

bool SDLFontAsset::load(SDL_Renderer* renderer) {
    if (m_isLoaded) return true;

    if (!renderer) {
        std::cerr << "SDLFontAsset: renderer is null\n";
        return false;
    }

    if (TTF_WasInit() == 0) {
        if (TTF_Init() != 0) {
            std::cerr << "SDLFontAsset: failed to init SDL_ttf: "
                      << TTF_GetError() << "\n";
            return false;
        }
    }

    if (!loadGlyphsAndBuildAtlas(renderer)) {
        std::cerr << "SDLFontAsset: Failed to build atlas\n";
        return false;
    }

    m_isLoaded = true;
    return true;
}

void SDLFontAsset::unload() {
    if (!m_isLoaded) return;

    if (m_textureAtlas) {
        SDL_DestroyTexture(m_textureAtlas);
        m_textureAtlas = nullptr;
    }

    m_isLoaded = false;
}

/********************************************************************
 * loadGlyphsAndBuildAtlas
 ********************************************************************/
bool SDLFontAsset::loadGlyphsAndBuildAtlas(SDL_Renderer* renderer) {
    TTF_Font* font = TTF_OpenFont(m_path.c_str(), m_pointSize);
    if (!font) {
        std::cerr << "SDLFontAsset: Could not load font "
                  << m_path << ": " << TTF_GetError() << "\n";
        return false;
    }

    // Determine atlas cell size
    int maxW = 0, maxH = 0;
    for (char c = 32; c < 127; c++) {
        int minx, maxx, miny, maxy, advance;
        if (TTF_GlyphMetrics(font, c, &minx, &maxx, &miny, &maxy, &advance) == 0) {
            maxW = std::max(maxW, maxx - minx);
            maxH = std::max(maxH, maxy - miny);
        }
    }

    const int cols = 16;
    const int rows = 6;

    m_atlasWidth  = cols * maxW;
    m_atlasHeight = rows * maxH;

    SDL_Surface* atlas = SDL_CreateRGBSurfaceWithFormat(
        0,
        m_atlasWidth,
        m_atlasHeight,
        32,
        SDL_PIXELFORMAT_RGBA32
    );

    if (!atlas) {
        std::cerr << "SDLFontAsset: Failed to create atlas surface\n";
        TTF_CloseFont(font);
        return false;
    }

    SDL_FillRect(atlas, nullptr,
                 SDL_MapRGBA(atlas->format, 0, 0, 0, 0));

    int x = 0, y = 0;

    for (char c = 32; c < 127; c++) {
        renderGlyphToAtlas(atlas, c, x, y, font);

        int minx, maxx, miny, maxy, adv;
        TTF_GlyphMetrics(font, c, &minx, &maxx, &miny, &maxy, &adv);

        m_glyphs[c] = {
            adv, 0,
            minx, maxy,
            maxx - minx, maxy - miny
        };

        x += maxW;
        if (x + maxW > m_atlasWidth) {
            x = 0;
            y += maxH;
        }
    }

    m_textureAtlas = SDL_CreateTextureFromSurface(renderer, atlas);
    SDL_FreeSurface(atlas);
    TTF_CloseFont(font);

    if (!m_textureAtlas) {
        std::cerr << "SDLFontAsset: Failed to create texture: "
                  << SDL_GetError() << "\n";
        return false;
    }

    return true;
}

/********************************************************************
 * renderGlyphToAtlas
 ********************************************************************/
bool SDLFontAsset::renderGlyphToAtlas(SDL_Surface* atlas,
                                      char c,
                                      int x, int y,
                                      TTF_Font* font)
{
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface* glyph = TTF_RenderGlyph_Blended(font, c, white);

    if (!glyph)
        return false;

    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = glyph->w;
    dst.h = glyph->h;

    SDL_BlitSurface(glyph, nullptr, atlas, &dst);
    SDL_FreeSurface(glyph);
    return true;
}

} // namespace retronomicon::sdl::asset
