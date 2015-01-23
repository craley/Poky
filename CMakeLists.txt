cmake_minimum_required(VERSION 3.1)
project(pokedex)
add_subdirectory(sqlite)
add_executable(pokedex main.cpp imgui.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
include(FindPkgConfig)

pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_search_module(SDL2TTF REQUIRED SDL2_ttf>=2.0.0)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2TTF_LIBRARIES})

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
