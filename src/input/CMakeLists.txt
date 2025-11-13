file(GLOB_RECURSE SDL_INPUT_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-sdl-input OBJECT ${SDL_INPUT_PLUGIN})

target_include_directories(
    retronomicon-sdl-input PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_SDL_DIR}/include
    ${RETRO_SDL_DIR}/external/SDL2/include
)
