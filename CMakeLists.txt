cmake_minimum_required(VERSION 3.0)
project(expr VERSION 0.1.0)
configure_file(include/config.h.in config.h)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set (CMAKE_CXX_STANDARD 17)
set (CXX_STANDARD_REQUIRED ON)

BISON_TARGET(expr_parser src/parser.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cpp)
FLEX_TARGET(expr_scanner src/scanner.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cpp)
ADD_FLEX_BISON_DEPENDENCY(expr_scanner expr_parser)

include_directories(
        ${CMAKE_CURRENT_BINARY_DIR}
        include
        src
)

add_executable(${PROJECT_NAME}
        ${BISON_expr_parser_OUTPUTS}
        ${FLEX_expr_scanner_OUTPUTS}
        src/driver.cpp
        src/main.cpp
)

#add_executable(demo src/demo.cpp)
#target_link_libraries(demo ${PROJECT_NAME})
