# Game executable
cmake_minimum_required(VERSION 3.20)

# Game sources
set(GAME_SOURCES
    src/main.cc
)

add_executable(bge_game ${GAME_SOURCES})

target_include_directories(bge_game
    PRIVATE
        ${CMAKE_SOURCE_DIR}/engine/src
)

target_link_libraries(bge_game
    PRIVATE
        bge_engine
)

if(BGE_ENABLE_TRACY)
    target_link_libraries(bge_game PRIVATE Tracy::TracyClient)
    target_compile_definitions(bge_game PRIVATE TRACY_ENABLE)
endif()

# Platform-specific link options
if(BGE_PLATFORM_MACOS)
    # On macOS, frameworks must be passed as separate -framework flags
    # Set LINK_FLAGS property directly to avoid CMake list joining issues
    set_target_properties(bge_game PROPERTIES
        LINK_FLAGS "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo -framework CoreMedia -framework VideoToolbox -framework Metal -framework QuartzCore -framework Foundation -framework OpenGL -Wl,-rpath,@loader_path/../lib"
    )
elseif(BGE_PLATFORM_LINUX)
    target_link_libraries(bge_game PRIVATE GL pthread dl)
    target_link_options(bge_game PRIVATE
        -Wl,-rpath,\$ORIGIN/../lib
    )
elseif(BGE_PLATFORM_WINDOWS)
    target_link_libraries(bge_game PRIVATE opengl32 gdi32 user32 shell32)
    target_link_options(bge_game PRIVATE
        /DYNAMICBASE /NXCOMPAT
    )
endif()

# Copy shaders to build directory
add_custom_command(TARGET bge_game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory $<TARGET_FILE_DIR:bge_game>/../shaders
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_BINARY_DIR}/shaders
        $<TARGET_FILE_DIR:bge_game>/../shaders
    COMMENT "Copying shaders to game output directory"
)

# Install
install(TARGETS bge_game
    RUNTIME DESTINATION bin
)