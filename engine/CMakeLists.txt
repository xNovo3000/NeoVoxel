cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE src "src/*.cpp")
add_library(neovoxel ${src})
target_include_directories(neovoxel PUBLIC "include")

target_precompile_headers(neovoxel PRIVATE "include/pch.hpp")
set_target_properties(neovoxel PROPERTIES CXX_STANDARD 23)

# spdlog
target_include_directories(neovoxel PUBLIC "vendor/spdlog/include")