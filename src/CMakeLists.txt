
find_package(BISON)
find_package(FLEX)

set(BISON_MainParser_COMPILE_FLAGS t)

BISON_TARGET(MainParser parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp )
FLEX_TARGET(MainScanner lexer.l  ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(MainScanner MainParser)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} )

add_executable(
    main main.cpp
    ${BISON_MainParser_OUTPUTS}
    ${FLEX_MainScanner_OUTPUTS}
)
