cmake_minimum_required(VERSION 3.20)

file(GLOB_RECURSE src "src/*.cpp")
add_executable(sandbox ${src})
target_include_directories(sandbox PRIVATE "include")

set_target_properties(sandbox PROPERTIES CXX_STANDARD 23)

target_link_libraries(sandbox PUBLIC engine)