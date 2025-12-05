file(GLOB_RECURSE SDL_ASSET_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-sdl-asset OBJECT ${SDL_ASSET_PLUGIN})

target_include_directories(
    retronomicon-sdl-asset PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_SDL_DIR}/include
    ${RETRO_SDL_DIR}/external/SDL2/include
    ${RETRO_SDL_DIR}/external/SDL2_mixer/include
)

target_link_libraries(retronomicon-sdl-asset
        retronomicon
        SDL2
        SDL2_mixer
        SDL2_ttf
        )

