message(STATUS "==== Processing ${CMAKE_CURRENT_LIST_FILE} ====")

# Add submodules inside lib/
add_subdirectory(graphics)
add_subdirectory(manager)
add_subdirectory(input)
add_subdirectory(asset)
add_subdirectory(audio)
#add_subdirectory(math)

# Define the main retronomicon engine library
add_library(retronomicon-sdl)

# Link subsystem libraries
target_link_libraries(retronomicon-sdl
    PUBLIC
        retronomicon
        retronomicon-sdl-graphics
        retronomicon-sdl-asset
        retronomicon-sdl-manager
        retronomicon-sdl-input
        retronomicon-sdl-audio
        SDL2
        SDL2_mixer
)

# Later you can easily extend here:
# add_subdirectory(physics)
# add_subdirectory(audio)
