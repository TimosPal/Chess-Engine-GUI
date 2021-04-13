cmake_minimum_required(VERSION 3.16)
project(Chess)

set(CMAKE_CXX_STANDARD 20)

# - - SFML - -
set(SFML_INCLUDE_DIR "C:\\SFML-2.5.1/include")
set(SFML_LIBRARY_DIR "C:\\SFML-2.5.1/lib")
set(SFML_DIR "C:\\SFML-2.5.1/lib/cmake/SFML")

link_directories(SFML_LIBRARY_DIR)
include_directories(SFML_INCLUDE_DIR)
find_package(SFML 2.5.1 COMPONENTS system window audio graphics)
# - - - - - -

add_subdirectory(Engine)

add_executable(Chess main.cpp Frontend/src/TextureManager.h Frontend/src/TextureManager.cpp Frontend/src/Game.h Frontend/src/Game.cpp Frontend/src/RenderingUtil.h Frontend/src/RenderingUtil.cpp)
target_link_libraries(Chess Engine sfml-graphics sfml-audio sfml-window sfml-system)