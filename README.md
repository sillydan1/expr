# expr
A variable environment manipulation expression parser written in C++20 with flex and bison.

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
    symbol_map_t env{};             // Initialize an environment
    driver drv{env};                // Initialize the expr driver with the environment
    if (!drv.parse("a := 32 + 2"))  // Parse your expressions
        std::cout << drv.result;    // Print the result
} catch(const std::exception& e) {  // Parsing went wrong. Maybe bad syntax, type error or identifier not in environment
    std::cout << e.what();          // Print what went wrong
}
```

## Compile
You should be able to compile with cmake like so:
```
mkdir bin && cd bin
cmake ..
make 
```
