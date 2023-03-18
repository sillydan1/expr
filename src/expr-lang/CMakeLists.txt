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
project(expr_lang VERSION 3.0.0)
set(CMAKE_CXX_STANDARD 20)
set(CXX_STANDARD_REQUIRED ON)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
set(${PROJECT_NAME}_BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR} CACHE STRING "${PROJECT_NAME}_BUILD_DIR" FORCE) # CPM compatibility
BISON_TARGET(expr_parser expr.y ${CMAKE_CURRENT_BINARY_DIR}/expr-parser.cpp)
FLEX_TARGET(expr_lexer expr.l ${CMAKE_CURRENT_BINARY_DIR}/expr-scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(expr_lexer expr_parser)
add_library(${PROJECT_NAME} SHARED 
    ${BISON_expr_parser_OUTPUTS} 
    ${FLEX_expr_lexer_OUTPUTS} 
    ast-factory.cpp 
    language-builder.cpp
)
target_include_directories(${PROJECT_NAME} PUBLIC 
    ${CMAKE_CURRENT_BINARY_DIR} 
    ${yaoverload_SOURCE_DIR}/include
    ${yatree_SOURCE_DIR}/include
    ${yatimer_SOURCE_DIR}/include
    ${yahashcombine_SOURCE_DIR}/include
    ${argvparse_SOURCE_DIR}/include
    ${FLEX_INCLUDE_DIRS}
    ${BISON_INCLUDE_DIRS}
    .. .)
target_link_libraries(${PROJECT_NAME} expr)

