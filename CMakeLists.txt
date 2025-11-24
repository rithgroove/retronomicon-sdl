cmake_minimum_required(VERSION 3.15)
project(retronomicon-sdl VERSION 0.1 LANGUAGES CXX)

# C++ settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Output folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(RETRO_SDL_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Retronomicon SDL include path")
set(RETRO_SDL_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include CACHE PATH "Retronomicon SDL include path")

# External libraries
add_subdirectory(external/SDL2)
set(SDL2MIXER_OPUS OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_WAVPACK OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_FLAC OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MIDI OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MOD OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MP3 OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MPG123 OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MOD_MODPLUG OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_MOD_XMP OFF CACHE BOOL "" FORCE)
set(SDL2MIXER_FLUIDSYNTH OFF CACHE BOOL "" FORCE)
# use STB vorbis
set(SDL2MIXER_VORBIS STB CACHE BOOL "" FORCE)
add_subdirectory(external/SDL2_mixer)

# Engine source (will add retronomicon-sdl library)
add_subdirectory(src)

# Public include paths
include_directories(
    ${RETRO_SDL_DIR}/include
    ${RETRO_SDL_DIR}/external/SDL2/include
    ${RETRO_SDL_DIR}/external/SDL2_mixer/include
)

# Optional: Message info
message(STATUS "Building retronomicon-sdl as a library ✅")
