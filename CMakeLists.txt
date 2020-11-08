
cmake_minimum_required(VERSION 3.17)

project(DungeonCrawl CXX)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 17)

find_package(BISON)
find_package(FLEX)



add_subdirectory(command_line)
add_subdirectory(dungeon)
add_subdirectory(fungeon)
add_subdirectory(game)
add_subdirectory(threads)




