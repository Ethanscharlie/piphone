cmake_minimum_required(VERSION 3.18)

project(piphone CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)

add_executable(piphone 
  src/main.cpp
)

target_include_directories(piphone PRIVATE
  src
)

target_link_libraries(piphone PRIVATE 
  SDL2::SDL2
)
