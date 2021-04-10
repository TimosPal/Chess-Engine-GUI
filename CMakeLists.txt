cmake_minimum_required(VERSION 3.16)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Engine)

add_executable(Chess main.cpp)
target_link_libraries(Chess Engine)