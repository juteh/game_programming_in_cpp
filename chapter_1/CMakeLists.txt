cmake_minimum_required(VERSION 3.20)
project(Chapter01)

find_package(SDL2 CONFIG REQUIRED)

add_executable(Chapter01 main.cpp Game.cpp Game.h)

target_link_libraries(Chapter01 PRIVATE SDL2::SDL2 SDL2::SDL2main)