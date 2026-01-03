cmake_minimum_required(VERSION 3.10)
project(cogurvrp)

# Forcing raylib to do not compile unecessary examples
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)

add_subdirectory(lib/raylib)

add_executable(cogurvrp main.cpp levels_array.cpp)

target_link_libraries(cogurvrp raylib m pthread dl rt)

target_include_directories(cogurvrp PRIVATE lib/raylib/src)