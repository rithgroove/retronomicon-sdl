#include "retronomicon/asset/sdl_font_asset.h"
#include <iostream>
#include <algorithm>

namespace retronomicon::sdl::asset {

// ------------------------------------------------------------
// Utility
// ------------------------------------------------------------
static SDL_Surface* createBlankRGBA(int w, int h) {
    return SDL_CreateRGBSurfaceWithFormat(
        0, w, h, 32, SDL_PIXELFORMAT_RGBA32
    );
}

std::string SDLFontAsset::to_string() const {
    return "SDLFontAsset(name=" + m_name +
           ", path=" + m_path +
           ", size=" + std::to_string(m_pointSize) +
           ", atlas=" + std::to_string(m_atlasWidth) + "x" +
                        std::to_string(m_atlasHeight) + ")";
}

// ------------------------------------------------------------
// LOAD
// ------------------------------------------------------------
bool SDLFontAsset::load() {
    if (m_isLoaded) return true;

    if (TTF_WasInit() == 0 && TTF_Init() != 0) {
        std::cerr << "[SDLFontAsset] SDL_ttf init failed: " << TTF_GetError() << "\n";
        return false;
    }

    if (!loadGlyphs()) {
        std::cerr << "[SDLFontAsset] Failed to load glyphs.\n";
        return false;
    }

    if (!buildAtlas()) {
        std::cerr << "[SDLFontAsset] Failed to build atlas.\n";
        return false;
    }

    m_isLoaded = true;
    return true;
}

void SDLFontAsset::unload() {
    if (!m_isLoaded) return;

    for (auto& [c, gs] : m_surfaces) {
        if (gs.surf) SDL_FreeSurface(gs.surf);
    }
    m_surfaces.clear();

    m_pixels.clear();
    m_pixels.shrink_to_fit();

    m_atlasWidth = 0;
    m_atlasHeight = 0;

    m_glyphs.clear();

    m_isLoaded = false;
}

// ------------------------------------------------------------
// STEP 1: LOAD GLYPHS (metrics + individual surfaces)
// ------------------------------------------------------------
bool SDLFontAsset::loadGlyphs() {
    TTF_Font* font = TTF_OpenFont(m_path.c_str(), m_pointSize);
    if (!font) {
        std::cerr << "[SDLFontAsset] Failed to open font: "
                  << m_path << " (" << TTF_GetError() << ")\n";
        return false;
    }

    int maxW = 0, maxH = 0;

    for (char c = 32; c < 127; c++) {
        int minx, maxx, miny, maxy, advance;

        if (TTF_GlyphMetrics(font, c, &minx, &maxx, &miny, &maxy, &advance) != 0)
            continue;

        SDL_Color white = {255,255,255,255};
        SDL_Surface* glyphSurf = TTF_RenderGlyph_Blended(font, c, white);

        if (!glyphSurf)
            continue;

        int w = glyphSurf->w;
        int h = glyphSurf->h;

        maxW = std::max(maxW, w);
        maxH = std::max(maxH, h);

        m_surfaces[c] = {
            glyphSurf,
            w,
            h,
            minx,
            maxy,
            advance
        };
    }

    TTF_CloseFont(font);

    if (m_surfaces.empty()) {
        std::cerr << "[SDLFontAsset] No glyphs were rasterized.\n";
        return false;
    }

    return true;
}

// ------------------------------------------------------------
// STEP 2: PACK GLYPH SURFACES INTO ATLAS (RGBA pixel buffer)
// ------------------------------------------------------------
bool SDLFontAsset::buildAtlas() {
    if (m_surfaces.empty()) return false;

    // Simple fixed grid atlas (same as OpenGL stub)
    int cellSize = 0;
    for (auto& [c, gs] : m_surfaces)
        cellSize = std::max(cellSize, std::max(gs.width, gs.height));

    int cols = 16;
    int rows = (int)((m_surfaces.size() + cols - 1) / cols);

    m_atlasWidth  = cols * cellSize;
    m_atlasHeight = rows * cellSize;

    SDL_Surface* atlas = createBlankRGBA(m_atlasWidth, m_atlasHeight);
    if (!atlas) {
        std::cerr << "[SDLFontAsset] Failed to create atlas surface.\n";
        return false;
    }

    SDL_FillRect(atlas, nullptr, SDL_MapRGBA(atlas->format, 0, 0, 0, 0));

    int idx = 0;
    for (auto& [c, gs] : m_surfaces) {
        int col = idx % cols;
        int row = idx / cols;

        int x = col * cellSize;
        int y = row * cellSize;

        SDL_Rect dst = {x, y, gs.width, gs.height};
        SDL_BlitSurface(gs.surf, nullptr, atlas, &dst);

        // Fill glyph metrics
        auto& gm = m_glyphs[c];
        gm.width  = gs.width;
        gm.height = gs.height;
        gm.advanceX = gs.advance;
        gm.bearingX = gs.bearingX;
        gm.bearingY = gs.bearingY;

        gm.atlasX = x;
        gm.atlasY = y;

        gm.u0 = float(x) / m_atlasWidth;
        gm.v0 = float(y) / m_atlasHeight;
        gm.u1 = float(x + gs.width) / m_atlasWidth;
        gm.v1 = float(y + gs.height) / m_atlasHeight;

        idx++;
    }

    // Convert SDL_Surface → raw RGBA buffer
    m_pixels.resize(m_atlasWidth * m_atlasHeight * 4);
    memcpy(m_pixels.data(), atlas->pixels, m_pixels.size());

    SDL_FreeSurface(atlas);
    return true;
}

} // namespace retronomicon::sdl::asset
