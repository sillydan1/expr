cmake_minimum_required(VERSION 3.21)
project(expr VERSION 1.4.0)
include(cmake/CPM.cmake)
configure_file(src/config.h.in config.h)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

# OPTIONS
option(ENABLE_Z3 "Enables the download and compilation of the expr::z3_driver driver. OFF by default" OFF)

# DEPENDENCIES
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
CPMAddPackage("gh:yalibs/yaoverload@1.0.0")
CPMAddPackage("gh:yalibs/yatree@1.0.1")
CPMAddPackage("gh:yalibs/yatimer@1.0.0")
CPMAddPackage("gh:yalibs/yahashcombine@1.0.0")
CPMAddPackage("gh:sillydan1/argvparse@1.2.2")
if(ENABLE_Z3)
    CPMAddPackage("gh:Z3Prover/z3#z3-4.8.17")
endif()

BISON_TARGET(expr_parser src/parser/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(expr_scanner src/parser/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(expr_scanner expr_parser)

set(${PROJECT_NAME}_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "expr_BUILD_DIR" FORCE)
include_directories(${overload_SOURCE_DIR}/include)
include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        ${yaoverload_SOURCE_DIR}/include
        ${yatree_SOURCE_DIR}/include
        ${yatimer_SOURCE_DIR}/include
        ${yahashcombine_SOURCE_DIR}/include
        ${argvparse_SOURCE_DIR}/include
        ${z3_SOURCE_DIR}/src/api
        include
        src)
add_library(${PROJECT_NAME} SHARED
        ${BISON_expr_parser_OUTPUTS}
        ${FLEX_expr_scanner_OUTPUTS}
        src/drivers/interpreter.cpp
        src/drivers/compiler.cpp

        src/symbol_table.cpp
        src/operations/add.cpp
        src/operations/subtract.cpp
        src/operations/multiply.cpp
        src/operations/divide.cpp
        src/operations/modulo.cpp
        src/operations/pow.cpp
        src/operations/boolean.cpp)
if(ENABLE_Z3)
    target_sources(${PROJECT_NAME} PUBLIC src/drivers/z3_driver.cpp)
    target_link_libraries(${PROJECT_NAME} libz3)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ENABLE_Z3)
endif()

add_executable(${PROJECT_NAME}_demo src/main.cpp)
target_link_libraries(${PROJECT_NAME}_demo ${PROJECT_NAME} argvparse)
