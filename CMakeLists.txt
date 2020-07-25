
cmake_minimum_required(VERSION 3.17)

project(DungeonCrawl CXX)

set(CMAKE_CXX_STANDARD 14)

find_package(BISON)
find_package(FLEX)



add_subdirectory(command_line)
add_subdirectory(dungeon)
add_subdirectory(game)




