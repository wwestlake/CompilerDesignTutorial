BISON_TARGET(FungeonParser fungeon_parser.y ${CMAKE_CURRENT_BINARY_DIR}/fungeon_parser.cpp )
FLEX_TARGET(FungeonScanner fungeon_lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/fungeon_lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(FungeonScanner FungeonParser)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

add_library(fungeon STATIC
    fungeon_ast.h fungeon_ast.cpp
    visitable.h
    visitor.h
    utils.h utils.cpp
    pretty_print.h

    ${BISON_FungeonParser_OUTPUTS}
    ${FLEX_FungeonScanner_OUTPUTS}
    
)
