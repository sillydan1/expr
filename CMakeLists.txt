cmake_minimum_required(VERSION 3.0)
project(expr VERSION 0.1.0)
configure_file(src/config.h.in config.h)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD_REQUIRED ON)

BISON_TARGET(expr_parser src/rules.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(expr_scanner src/lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.cpp)
ADD_FLEX_BISON_DEPENDENCY(expr_scanner expr_parser)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        include
        src
)

message(STATUS ${BISON_expr_parser_OUTPUTS})
message(STATUS ${FLEX_expr_parser_OUTPUTS})
add_library(${PROJECT_NAME}
        ${BISON_expr_parser_OUTPUTS}
        ${FLEX_expr_parser_OUTPUTS}
)

#add_executable(demo src/demo.cpp)
#target_link_libraries(demo ${PROJECT_NAME})
