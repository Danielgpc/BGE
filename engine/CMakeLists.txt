# Engine library
cmake_minimum_required(VERSION 3.20)

# Platform-specific source
if(BGE_PLATFORM_MACOS)
    set(PLATFORM_SRC src/platform_mac.cc)
elseif(BGE_PLATFORM_LINUX)
    set(PLATFORM_SRC src/platform_linux.cc)
elseif(BGE_PLATFORM_WINDOWS)
    set(PLATFORM_SRC src/platform_win.cc)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

# Engine sources
set(ENGINE_SOURCES
    src/bge.cc
    src/shaders.cc
    src/image.cc
    ${PLATFORM_SRC}
)

# Engine headers
set(ENGINE_HEADERS
    src/bge.h
    src/config.h
    src/defines.h
    src/platform.h
    src/shaders.h
    src/image.h
    src/mutils.h
)

# Create library
add_library(bge_engine ${ENGINE_SOURCES} ${ENGINE_HEADERS})

# Target properties
target_include_directories(bge_engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(bge_engine
    PRIVATE
        BGE_ENGINE_EXPORTS
        BGE_SHADER_DIR="${CMAKE_BINARY_DIR}/shaders"
        _THREAD_SAFE
)

# Platform-specific defines for GLFW native access
if(BGE_PLATFORM_MACOS)
    target_compile_definitions(bge_engine PRIVATE GLFW_EXPOSE_NATIVE_COCOA)
elseif(BGE_PLATFORM_LINUX)
    target_compile_definitions(bge_engine PRIVATE GLFW_EXPOSE_NATIVE_X11)
elseif(BGE_PLATFORM_WINDOWS)
    target_compile_definitions(bge_engine PRIVATE GLFW_EXPOSE_NATIVE_WIN32)
endif()

# Link dependencies
target_link_libraries(bge_engine
    PUBLIC
        bgfx::bgfx
        bgfx::bx
        bgfx::bimg
        glfw
        imgui::imgui
        spdlog::spdlog
        glm::glm
        flecs::flecs
)

if(BGE_ENABLE_TRACY)
    target_link_libraries(bge_engine PUBLIC Tracy::TracyClient)
    target_compile_definitions(bge_engine PRIVATE TRACY_ENABLE)
endif()

# Platform-specific link options
if(BGE_PLATFORM_MACOS)
    # On macOS, frameworks must be passed as separate -framework flags
    # Set LINK_FLAGS property directly to avoid CMake list joining issues
    set_target_properties(bge_engine PROPERTIES
        LINK_FLAGS "-framework Cocoa -framework IOKit -framework CoreFoundation -framework CoreVideo -framework CoreMedia -framework VideoToolbox -framework Metal -framework QuartzCore -framework Foundation -framework OpenGL -Wl,-rpath,@loader_path"
    )
elseif(BGE_PLATFORM_LINUX)
    target_link_libraries(bge_engine PRIVATE GL pthread dl)
    target_link_options(bge_engine PRIVATE
        -Wl,-rpath,\$ORIGIN
    )
elseif(BGE_PLATFORM_WINDOWS)
    target_link_libraries(bge_engine PRIVATE opengl32 gdi32 user32 shell32)
    target_link_options(bge_engine PRIVATE
        /DYNAMICBASE /NXCOMPAT
    )
endif()

# Visibility
if(NOT MSVC)
    target_compile_options(bge_engine PRIVATE -fvisibility=hidden)
endif()

# Install
install(TARGETS bge_engine
    EXPORT BGETargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

# Alias for convenience
add_library(BGE::engine ALIAS bge_engine)