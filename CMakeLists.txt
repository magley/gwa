cmake_minimum_required(VERSION 3.12)
project("gwa")
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE gwa_src CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE gwa_src CONFIGURE_DEPENDS "src/*/*.cpp")
include_directories(src/)
include_directories(include/)

add_executable(gwa "src/main.cpp" ${gwa_src})