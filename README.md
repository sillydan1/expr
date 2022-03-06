# EXPR
A variable environment manipulation expression parser written in C++20 with flex and bison.

## Features
The parser rules have two different modes:
 1. Either evaluate a string of assignment expressions e.g. `a := 1; b := 2` or
 2. Evaluate a single expression with result stored in `driver.result["expression_result"]`

## Examples
This project comes with an demo example command line interface called `expr_demo` so that you
can test if the project supports your expression syntax. This example cli can 
```
./expr_demo -
a := (1 + 2 + 3 + 4);
b := one + two;
^D
a :-> 6 i
b :-> 3 i
```

You can also use the project directly in code like so: 
```c++
try {                               // Errors are handled with exceptions
    symbol_table_t env{};             // Initialize an environment
    driver drv{env};                // Initialize the expr driver with the environment
    if (!drv.parse("a := 32 + 2"))  // Parse your expressions
        std::cout << drv.result;    // Print the result
} catch(const std::exception& e) {  // Parsing went wrong. Maybe bad syntax, type error or identifier not in environment
    std::cout << e.what();          // Print what went wrong
}
```

## Use CMake
This project is compiled through cmake, so you can simply configure the project and make it like so:
```shell
mkdir bin && cd bin
cmake ..
make
```
If you want to use the project in your own cmake project, simply include `expr` as a subdirectory and link
with the `libexpr` library. The project is also tagged with release versions to be compatible with [cpm](https://github.com/cpm-cmake/CPM.cmake), 
so if your project uses that, simply include the project like so:
```cmake
CPMAddPackage("gh:sillydan1/expr@latest")
```
