cmake_minimum_required(VERSION 3.21)
project(expr_lang VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
set(${PROJECT_NAME}_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "${PROJECT_NAME}_BUILD_DIR" FORCE) # CPM compatibility
BISON_TARGET(expr_parser expr.y ${CMAKE_BINARY_DIR}/expr-parser.cpp)
FLEX_TARGET(expr_lexer expr.l ${CMAKE_BINARY_DIR}/expr-scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(expr_lexer expr_parser)
add_library(${PROJECT_NAME} SHARED ${BISON_expr_parser_OUTPUTS} ${FLEX_expr_lexer_OUTPUTS})

