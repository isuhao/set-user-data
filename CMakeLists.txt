cmake_minimum_required(VERSION 3.1)
project(display_dispatch)

set(CMAKE_CXX_STANDARD 11)

file(GLOB SOURCE_FILES "*.cpp")
file(GLOB HEADER_FILES "*.hpp")

add_executable(display_dispatch ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(display_dispatch wayland-client)