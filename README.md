# EXPR
A variable environment manipulation expression parser written in C++20 with flex and bison.
### Feature set
 1. Evaluate a string of assignment expressions e.g. `a := 1; b := 2`
 2. Strongly typed `string, int, float` and `bool`
    1. `int` and `float` are implicitly cast in operations
 3. Evaluate a single expression with result stored in `interpreter.result["expression_result"]`
 4. Compile an expression into a syntax tree with the `expr::compiler` and evaluate the tree with `expr::interpreter`
 5. If compiled with the cmake option `ENABLE_Z3` you can leverage the [z3 theorem prover](https://github.com/Z3Prover/z3) to solve a satisfiability query
 6. Results are expressed in symbol-table variable assignments

## Compilation Instructions
This project is compiled through cmake, so you can simply configure the project and make it like so:
```shell
mkdir bin && cd bin
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_Z3=ON  # z3 theorem prover is an opt-in feature. Note that compilation will take much longer if enabled
make
```
Note that because the flex/bison parser is generated at compile-time, your editor might complain about some invalid
and/or incomplete types before you've called `make`. To fix this, simply call `make` once, and the `yy::parser` type should
be generated for you.

If you want to use the project in your own cmake project, simply include `expr` as a subdirectory and link
with the `libexpr` library. The project is also tagged with release versions to be compatible with [cpm](https://github.com/cpm-cmake/CPM.cmake),
so if your project uses that, simply include the project like so:
```cmake
CPMAddPackage("gh:sillydan1/expr@1.4.0")
```

## Examples
This project comes with a demo command line interface called `expr_demo` so that you
can test if the project supports your expression syntax.  
```
$ ./expr_demo -

a := (1 + 2 + 3 + 4);
b := one + two;
^D
a :-> 6 i
b :-> 3 i
```

You can also use the project directly in code like so: 
```c++
using namespace expr;              // All elements are in the expr:: namespace
symbol_table_t env{};              // Initialize an environment
interpreter drv{env};              // Initialize the expr interpreter with the environment
if (drv.parse("a := 32 + 2") == 0) // Parse your expressions
    std::cout << drv.result;       // Print the resulting symbol table
else                               // Parsing went wrong. Maybe bad syntax, type error or identifier not in environment
    std::cout << drv.error;        // Print what went wrong
```

### Using The `expr::z3_driver`
If enabled, expr includes an integration with the [z3 sat solver](https://github.com/Z3Prover/z3). 
By providing an initial environment and a boolean expression, the `expr::z3_driver` can provide you with an assignment 
of those variables, such that the boolean expression evaluates to `true`.
This is best explained with an example. The demo binary is a fine place to explore this driver:
```shell
$ ./expr_demo --driver z3 \
              --environment "a := false; b := false; c := false" \
              --expression "a == !(b || c)" 
```
We ask z3 to find a solution where `a` would be the same value as the expression `!(b || c)`. 
Below is the result of the above query.
```
result: 
a :-> false b
b :-> true b
c :-> false b
```
There are in fact more than one solution to this query - you could just as well set `c :-> true` and the query would still
be satisfied. 
If you want to iterate over all solutions, you can just append every solution you get to your query with a `!`-operator in front of it, and run the program again.

If you provide a query, where no assignment of the variables would lead to `true`, the driver will exclaim `"unsat"` in the terminal, and the `result` symbol table will be empty:
```shell
$ ./expr_demo --driver z3 \
              --environment "a := false" \
              --expression "a != a" # not satisfiable
```
```
unsat
result:

```
