file(GLOB_RECURSE SDL_MANAGER_PLUGIN CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(retronomicon-sdl-manager OBJECT ${SDL_MANAGER_PLUGIN})

target_include_directories(
    retronomicon-sdl-manager PUBLIC
    ${RETRO_DIR}/include
    ${RETRO_SDL_DIR}/include
    ${RETRO_SDL_DIR}/external/SDL2/include
)
