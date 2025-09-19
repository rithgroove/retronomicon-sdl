file(GLOB_RECURSE SDL_GRAPHIC_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-sdl-graphics OBJECT ${SDL_GRAPHIC_PLUGIN})

target_include_directories(
    retronomicon-sdl-graphics PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_SDL_DIR}/include
    ${RETRO_SDL_DIR}/external/SDL2/include
)
