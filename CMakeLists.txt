cmake_minimum_required(VERSION 3.12)
set(PROJECT_NAME "gwa")
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE gwa_src CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE gwa_src CONFIGURE_DEPENDS "src/*/*.cpp")
include_directories(src/)
include_directories(include/)

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)
link_directories(${LIB_DIR})

add_executable(${PROJECT_NAME} "src/main.cpp" ${gwa_src})

target_link_libraries(${PROJECT_NAME}
    -lmingw32 
    ${LIB_DIR}/SDL2/libSDL2main.a 
    ${LIB_DIR}/SDL2/SDL2.dll
)
