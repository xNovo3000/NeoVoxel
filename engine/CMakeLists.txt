cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE src "src/*.cpp")
add_library(engine ${src})
target_include_directories(engine PUBLIC "include")

target_precompile_headers(engine PRIVATE "include/pch.hpp")
set_target_properties(engine PROPERTIES CXX_STANDARD 23)

target_compile_definitions(engine PUBLIC NV_BUILD_TYPE_DEBUG=0)
target_compile_definitions(engine PUBLIC NV_BUILD_TYPE_PROFILE=1)
target_compile_definitions(engine PUBLIC NV_BUILD_TYPE_RELEASE=2)
target_compile_definitions(engine PUBLIC NV_BUILD_TYPE=$<$<CONFIG:Debug>:NV_BUILD_TYPE_DEBUG>$<$<CONFIG:RelWithDebInfo>:NV_BUILD_TYPE_PROFILE>$<$<CONFIG:Release>:NV_BUILD_TYPE_RELEASE>)

# glad
find_package(OpenGL)
target_link_libraries(engine PRIVATE OpenGL::GL)
target_include_directories(engine PRIVATE "vendor/glad/include")

# glfw
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_directories(engine PUBLIC "vendor/glfw/lib/macos-universal")
    target_link_libraries(engine PUBLIC "glfw3")
    target_link_libraries(engine PUBLIC "-framework Cocoa")
    target_link_libraries(engine PUBLIC "-framework IOKit")
endif ()
target_include_directories(engine PRIVATE "vendor/glfw/include")
target_compile_definitions(engine PRIVATE GLFW_INCLUDE_NONE)

# glm
target_include_directories(engine PUBLIC "vendor/glm/include")

# openal-soft
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_link_directories(engine PUBLIC "vendor/openal-soft/lib/macos-universal")
    target_link_libraries(engine PUBLIC "alsoft.common")
endif ()
target_include_directories(engine PRIVATE "vendor/openal-soft/include")

# spdlog
target_include_directories(engine PUBLIC "vendor/spdlog/include")