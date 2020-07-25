

BISON_TARGET(MainParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp )
FLEX_TARGET(MainScanner lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MainScanner MainParser)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )
set(BISON_MainParser_COMPILE_FLAGS t)

add_library( command_line
    ../include/ast.h
    ${BISON_MainParser_OUTPUTS}
    ${FLEX_MainScanner_OUTPUTS}
)
