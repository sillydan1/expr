# MIT License
#
# Copyright (c) 2022 Asger Gitz-Johansen
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
cmake_minimum_required(VERSION 3.18)
project(expr VERSION 3.0.3)
include(cmake/CPM.cmake)
configure_file(src/config.h.in config.h)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)

# OPTIONS
option(ENABLE_Z3 "Enables the download and compilation of the expr::z3_driver driver. OFF by default" OFF)

# DEPENDENCIES
# library dependencies
CPMAddPackage("gh:yalibs/yaoverload@1.0.0")
CPMAddPackage("gh:yalibs/yahashcombine@1.0.0")
CPMAddPackage("gh:yalibs/yatree@1.2.2")
CPMAddPackage("gh:yalibs/yauuid@1.0.1")
# demo dependencies
CPMAddPackage("gh:yalibs/yatimer@1.0.0")
CPMAddPackage("gh:sillydan1/argvparse@1.2.3")

if(ENABLE_Z3)
    include(cmake/Z3.cmake)
    set(Z3_VERSION_NUMBER z3-4.12.2)
    get_z3_zip_file(${Z3_VERSION_NUMBER})
    CPMAddPackage(NAME z3 VERSION ${Z3_VERSION_NUMBER} URL ${z3vstr})
endif()

set(${PROJECT_NAME}_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "expr_BUILD_DIR" FORCE)

add_library(${PROJECT_NAME} SHARED
        src/symbol/clock.cpp
        src/symbol/symbol_table.cpp
        src/operations/add.cpp
        src/operations/subtract.cpp
        src/operations/multiply.cpp
        src/operations/divide.cpp
        src/operations/modulo.cpp
        src/operations/pow.cpp
        src/operations/boolean.cpp
        src/driver/evaluator.cpp
        )

target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/src
        ${yatree_SOURCE_DIR}/include
        ${yatimer_SOURCE_DIR}/include
        ${yaoverload_SOURCE_DIR}/include
        ${yahashcombine_SOURCE_DIR}/include
        ${yauuid_SOURCE_DIR}/include
        ${argvparse_SOURCE_DIR}/include
        ${z3_SOURCE_DIR}/src/api/c++
        ${z3_SOURCE_DIR}/include
        src/symbol
        include
        src)

add_library(${PROJECT_NAME}_generic_driver SHARED src/generic-driver.cpp)
target_include_directories(${PROJECT_NAME}_generic_driver PUBLIC
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}/src
        ${yatree_SOURCE_DIR}/include
        ${yatimer_SOURCE_DIR}/include
        ${yaoverload_SOURCE_DIR}/include
        ${yahashcombine_SOURCE_DIR}/include
        ${argvparse_SOURCE_DIR}/include
        ${z3_SOURCE_DIR}/src/api/c++
        ${z3_SOURCE_DIR}/include
        src/symbol
        include
        src)

add_subdirectory(src/expr-lang)

if(ENABLE_Z3)
    target_sources(${PROJECT_NAME} PUBLIC src/driver/z3/z3-driver.cpp)
    target_link_directories(${PROJECT_NAME} PUBLIC ${z3_SOURCE_DIR}/bin)
    target_link_libraries(${PROJECT_NAME} z3)
    target_compile_definitions(${PROJECT_NAME} PUBLIC ENABLE_Z3)
    add_custom_target(copy_z3 COMMAND ${CMAKE_COMMAND} -E copy ${z3_SOURCE_DIR}/bin/libz3.* ${CMAKE_BINARY_DIR})
    add_dependencies(${PROJECT_NAME} copy_z3)
endif()

add_executable(${PROJECT_NAME}_demo src/main.cpp)
set_target_properties(${PROJECT_NAME}_demo PROPERTIES RPATH ${z3_SOURCE_DIR}/bin)
target_link_libraries(${PROJECT_NAME}_demo ${PROJECT_NAME} expr_lang argvparse ${PROJECT_NAME}_generic_driver)
target_link_libraries(${PROJECT_NAME}_generic_driver expr_lang)

