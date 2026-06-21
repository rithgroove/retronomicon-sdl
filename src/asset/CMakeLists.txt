add_library(retronomicon-sdl-asset OBJECT
    ${CMAKE_CURRENT_SOURCE_DIR}/sdl_font_asset.cpp
)

target_include_directories(
    retronomicon-sdl-asset PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_SDL_DIR}/include
    ${RETRO_SDL_DIR}/external/SDL2/include
)

target_link_libraries(retronomicon-sdl-asset
        retronomicon
        SDL2
        SDL2_ttf
        )
