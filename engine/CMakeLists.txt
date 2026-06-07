cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE src "src/*.cpp")
add_library(neovoxel ${src})
target_include_directories(neovoxel PUBLIC "include")

target_precompile_headers(neovoxel PRIVATE "include/pch.hpp")
set_target_properties(neovoxel PROPERTIES CXX_STANDARD 23)

target_compile_definitions(neovoxel PUBLIC NV_BUILD_TYPE_DEBUG=0)
target_compile_definitions(neovoxel PUBLIC NV_BUILD_TYPE_PROFILE=1)
target_compile_definitions(neovoxel PUBLIC NV_BUILD_TYPE_RELEASE=2)
target_compile_definitions(neovoxel PUBLIC NV_BUILD_TYPE=$<$<CONFIG:Debug>:NV_BUILD_TYPE_DEBUG>$<$<CONFIG:RelWithDebInfo>:NV_BUILD_TYPE_PROFILE>$<$<CONFIG:Release>:NV_BUILD_TYPE_RELEASE>)

# glm
target_include_directories(neovoxel PUBLIC "vendor/glm/include")

# spdlog
target_include_directories(neovoxel PUBLIC "vendor/spdlog/include")