message(STATUS "==== Processing ${CMAKE_CURRENT_LIST_FILE} ====")

# Add submodules inside lib/
add_subdirectory(graphics)
#add_subdirectory(math)

# Define the main retronomicon engine library
add_library(retronomicon-sdl)

# Link subsystem libraries
target_link_libraries(retronomicon-sdl
    PUBLIC
        retronomicon-sdl-graphics
        SDL2
)

# Later you can easily extend here:
# add_subdirectory(physics)
# add_subdirectory(audio)
