cmake_minimum_required(VERSION 3.1)
project(display_dispatch)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
    set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11")
else ()
    set(CMAKE_CXX_FLAGS "-O3 -std=c++11")
endif ()

file(GLOB SOURCE_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")

add_executable(display_dispatch ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(display_dispatch wayland-client)