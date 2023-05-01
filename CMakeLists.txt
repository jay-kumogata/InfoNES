cmake_minimum_required(VERSION 3.1)

project(InfoNES)

find_package(SDL REQUIRED SDL)
include_directories(${SDL_INCLUDE_DIRS})

file(GLOB SRCS
        src/*.cpp
        src/sdl/*.cpp
        )
add_executable(${PROJECT_NAME} ${SRCS})
target_link_libraries(${PROJECT_NAME} ${SDL_LIBRARIES})
