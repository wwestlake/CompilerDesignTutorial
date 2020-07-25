BISON_TARGET(DungeonParser dungeon_parser.y ${CMAKE_CURRENT_BINARY_DIR}/dungeon_parser.cpp )
FLEX_TARGET(DungeonScanner dungeon_lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/dungeon_lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(DungeonScanner DungeonParser)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

add_library(dungeon STATIC
    ${BISON_DungeonParser_OUTPUTS}
    ${FLEX_DungeonScanner_OUTPUTS}
)
