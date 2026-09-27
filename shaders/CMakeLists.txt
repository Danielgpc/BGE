# Shader compilation
cmake_minimum_required(VERSION 3.20)

# Shader source files
file(GLOB VS_SRCS "vs_*.sc")
file(GLOB FS_SRCS "fs_*.sc")

if(NOT VS_SRCS AND NOT FS_SRCS)
    message(WARNING "No shader sources found in ${CMAKE_CURRENT_SOURCE_DIR}")
    return()
endif()

# Shader output directory
set(SHADER_OUTPUT_DIR "${CMAKE_BINARY_DIR}/shaders")

# Shader APIs to compile for (based on platform)
if(BGE_PLATFORM_MACOS)
    set(SHADER_APIS "metal;spirv;glsl;essl")
else()
    set(SHADER_APIS "spirv;glsl;essl;dxbc;dxil")
endif()

# Platform mapping for shaderc
set(SHADERC_PLATFORM_METAL "osx")
set(SHADERC_PLATFORM_SPIRV "linux")
set(SHADERC_PLATFORM_GLSL "linux")
set(SHADERC_PLATFORM_ESSL "android")
set(SHADERC_PLATFORM_DXBC "windows")
set(SHADERC_PLATFORM_DXIL "windows")

# Profile mapping for shaderc
set(SHADERC_PROFILE_METAL "metal")
set(SHADERC_PROFILE_SPIRV "spirv")
set(SHADERC_PROFILE_GLSL "430")
set(SHADERC_PROFILE_ESSL "300_es")
set(SHADERC_PROFILE_DXBC "vs_5_0")
set(SHADERC_PROFILE_DXIL "vs_6_0")

# Find shaderc
if(NOT BGFX_SHADERC)
    find_program(BGFX_SHADERC shaderc
        PATHS "${bgfx_BINARY_DIR}/bin" "${bgfx_INSTALL_DIR}/bin" "${CMAKE_BINARY_DIR}/../third_party/bgfx/.build/${BGFX_ARCH}/bin"
        DOC "bgfx shader compiler"
    )
endif()

if(NOT BGFX_SHADERC)
    message(WARNING "shaderc not found - shaders will not be compiled. Build bgfx with tools first.")
    return()
endif()

# Include path for bgfx shader headers
set(BGFX_SHADER_INCLUDE "${bgfx_SOURCE_DIR}/include" CACHE PATH "bgfx shader include path")

# Function to add shader compilation
function(add_shader_target api_name vs_src fs_src)
    set(platform_var "SHADERC_PLATFORM_${api_name}")
    set(profile_var "SHADERC_PROFILE_${api_name}")
    set(platform ${${platform_var}})
    set(profile ${${profile_var}})

    set(api_output_dir "${SHADER_OUTPUT_DIR}/${api_name}")

    # Vertex shaders
    foreach(vs ${vs_src})
        get_filename_component(vs_name ${vs} NAME_WE)
        set(bin_file "${api_output_dir}/${vs_name}.bin")
        add_custom_command(
            OUTPUT ${bin_file}
            COMMAND ${BGFX_SHADERC}
                --platform ${platform}
                -p ${profile}
                -i ${BGFX_SHADER_INCLUDE}
                --type vertex
                -o ${bin_file}
                -f ${CMAKE_CURRENT_SOURCE_DIR}/${vs}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${vs} ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc ${BGFX_SHADERC}
            COMMENT "Compiling ${vs} for ${api_name}"
            VERBATIM
        )
        list(APPEND SHADER_BINS ${bin_file})
    endforeach()

    # Fragment shaders
    foreach(fs ${fs_src})
        get_filename_component(fs_name ${fs} NAME_WE)
        set(bin_file "${api_output_dir}/${fs_name}.bin")
        add_custom_command(
            OUTPUT ${bin_file}
            COMMAND ${BGFX_SHADERC}
                --platform ${platform}
                -p ${profile}
                -i ${BGFX_SHADER_INCLUDE}
                --type fragment
                -o ${bin_file}
                -f ${CMAKE_CURRENT_SOURCE_DIR}/${fs}
            DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${fs} ${CMAKE_CURRENT_SOURCE_DIR}/varying.def.sc ${BGFX_SHADERC}
            COMMENT "Compiling ${fs} for ${api_name}"
            VERBATIM
        )
        list(APPEND SHADER_BINS ${bin_file})
    endforeach()

    # Ensure output directory exists
    add_custom_command(
        OUTPUT ${api_output_dir}/.dir_created
        COMMAND ${CMAKE_COMMAND} -E make_directory ${api_output_dir}
        COMMAND ${CMAKE_COMMAND} -E touch ${api_output_dir}/.dir_created
        COMMENT "Creating shader output directory: ${api_output_dir}"
    )
    list(APPEND SHADER_BINS ${api_output_dir}/.dir_created)
endfunction()

# Add shader compilation for each API
foreach(api ${SHADER_APIS})
    string(TOUPPER ${api} API_UPPER)
    add_shader_target(${API_UPPER} "${VS_SRCS}" "${FS_SRCS}")
endforeach()

# Custom target for all shaders
add_custom_target(shaders ALL DEPENDS ${SHADER_BINS})

# Also copy to install directory
install(DIRECTORY ${SHADER_OUTPUT_DIR}/
    DESTINATION shaders
    FILES_MATCHING PATTERN "*.bin"
)