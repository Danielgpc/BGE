cmake_minimum_required(VERSION 3.20)
project(BGE
    VERSION 0.1.0
    LANGUAGES CXX C
    DESCRIPTION "BGE Game Engine"
)

# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Policy settings
cmake_policy(SET CMP0077 NEW)  # option() honors normal variables
cmake_policy(SET CMP0091 NEW)  # target_link_libraries() handles transitive deps

# Build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Default to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# vcpkg toolchain (if provided)
if(DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "vcpkg toolchain file")
    message(STATUS "Using vcpkg toolchain: ${CMAKE_TOOLCHAIN_FILE}")
endif()

# Options
option(BGE_BUILD_SHARED_LIBS "Build shared libraries" ON)
option(BGE_BUILD_TOOLS "Build shader compilation tools" ON)
option(BGE_ENABLE_ASAN "Enable AddressSanitizer (Debug builds)" ON)
option(BGE_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer (Debug builds)" ON)
option(BGE_ENABLE_TRACY "Enable Tracy profiler integration" ON)

# Platform detection
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(BGE_PLATFORM_MACOS 1)
    set(BGE_PLATFORM_NAME "macos")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(BGE_PLATFORM_LINUX 1)
    set(BGE_PLATFORM_NAME "linux")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(BGE_PLATFORM_WINDOWS 1)
    set(BGE_PLATFORM_NAME "windows")
else()
    message(FATAL_ERROR "Unsupported platform: ${CMAKE_SYSTEM_NAME}")
endif()

# Architecture detection
if(CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(BGE_ARCH "arm64")
else()
    set(BGE_ARCH "x64")
endif()

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX /permissive- /Zc:__cplusplus /Zc:inline /Zc:throwingNew)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Wshadow -Wnon-virtual-dtor -Wcast-align -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wnull-dereference -Wdouble-promotion -Wformat=2)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-Wno-unknown-warning-option -Wno-unused-template)
    endif()
endif()

# Sanitizers for Debug builds
if(BGE_ENABLE_ASAN OR BGE_ENABLE_UBSAN)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        if(BGE_ENABLE_ASAN)
            add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
            add_link_options(-fsanitize=address)
        endif()
        if(BGE_ENABLE_UBSAN)
            add_compile_options(-fsanitize=undefined -fno-sanitize-recover=all)
            add_link_options(-fsanitize=undefined)
        endif()
        add_definitions(-DBGE_DEBUG)
    endif()
endif()

# Platform-specific link libraries
if(BGE_PLATFORM_MACOS)
    set(BGE_PLATFORM_LIBS
        Cocoa IOKit CoreFoundation CoreVideo CoreMedia VideoToolbox Metal QuartzCore Foundation OpenGL
    )
elseif(BGE_PLATFORM_LINUX)
    set(BGE_PLATFORM_LIBS GL pthread dl)
elseif(BGE_PLATFORM_WINDOWS)
    set(BGE_PLATFORM_LIBS opengl32 gdi32 user32 shell32)
endif()

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/engine/src)

# Find vcpkg packages
find_package(bgfx REQUIRED)
find_package(glfw3 REQUIRED)
find_package(imgui REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)
# stb is header-only, no CMake targets - use include directory
# find_package(stb REQUIRED)
find_package(flecs REQUIRED)
find_package(Jolt REQUIRED)
find_package(meshoptimizer REQUIRED)
find_package(basisu REQUIRED)

if(BGE_ENABLE_TRACY)
    find_package(tracy REQUIRED)
endif()

# Shader compilation tool (shaderc from bgfx)
find_program(BGFX_SHADERC shaderc
    PATHS "${bgfx_BINARY_DIR}/bin" "${bgfx_INSTALL_DIR}/bin"
    DOC "bgfx shader compiler"
)

# Add subdirectories
add_subdirectory(engine)
add_subdirectory(game)
add_subdirectory(shaders)

# Installation (handled in engine/CMakeLists.txt)

# Custom targets (shaders target is defined in shaders/CMakeLists.txt)
add_custom_target(assets DEPENDS ${ASSET_TARGETS})

# Print summary
message(STATUS "")
message(STATUS "=== BGE Build Configuration ===")
message(STATUS "Platform: ${BGE_PLATFORM_NAME} (${BGE_ARCH})")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Shared Libs: ${BGE_BUILD_SHARED_LIBS}")
message(STATUS "Tracy: ${BGE_ENABLE_TRACY}")
message(STATUS "ASan: ${BGE_ENABLE_ASAN}")
message(STATUS "UBSan: ${BGE_ENABLE_UBSAN}")
message(STATUS "Shader Compiler: ${BGFX_SHADERC}")
message(STATUS "================================")