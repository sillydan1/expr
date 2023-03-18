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
    -DENABLE_Z3=ON  # z3 theorem prover is an opt-in feature
make
```
Note that because the flex/bison parser is generated at compile-time, your editor might complain about some invalid
and/or incomplete types before you've called `make`. To fix this, simply call `make` once, and the `yy::parser` type should
be generated for you.

If you want to use the project in your own cmake project, simply include `expr` as a subdirectory and link
with the `libexpr` library. The project is also tagged with release versions to be compatible with [cpm](https://github.com/cpm-cmake/CPM.cmake),
so if your project uses that, simply include the project like so:
```cmake
CPMAddPackage("gh:sillydan1/expr@3.0.0")
```

## Examples
Disclaimer: The examples in this section may be outdated. Look at [main.cpp](src/main.cpp) for an in-depth example of how to use the library.

This project comes with a demo command line interface called `expr_demo` so that you
can test if the project supports your expression syntax.  
```
$ ./expr_demo -m -
provide an environment. End with <<EOF>> (ctrl+d):
<<
a := 32
>>
provide an expression. End with <<EOF>> (ctrl+d):
<<
b := a / 2
>>
ast:
        > b :-> ROOT[/[a 2 i ]]
evaluated:
        > b :-> 16 i
z3 sat check: (apply these changes to satisfy the expression)
        > not a raw rhs, cannot check for sat
```

You can also use the project directly in code like so: 
```c++
expr::symbol_table_t env{};         // provide an environment for identifier lookup(s)
expr::generic_driver d{};           // instantiate default driver (make sure to link with libexpr_generic_driver.so for this
d.parse_expressions("a := 32 + 2"); // parse your expression
auto result = d.get_symbols(env);   // extract the symbol-table 
```

If you want more direct control, or want to inject some custom code (e.g. override the `add/+` operator), you can use the direct API:
```c++
using namespace expr;                                  // All elements are in the expr:: namespace
std::string expression = "a := 32+2";                  // Some expression string
std::stringstream stream{expression};                  // inputs must be wrapped in a c++ stream
ast_factory factory{};                                 // Initialize an overridable ast factory
declaration_tree_builder builder{};                    // Initialize an overridable tree builder
scanner sc{stream, std::cerr, &factory};               // Initialize scanner with input stream - write errors to std::cerr
parser_args args{expression, &sc, &factory, &builder}; // Wrap parser arguments
if(parser{args}.parse() != 0)                          // Actually parse the expression(s)
    throw std::logic_error("");                        // something bad happened either throw or handle the error
auto result = builder.build();                         // build the resulting AST(s)

// Now, we can evaluate our built AST(s)
symbol_table_t env{};                                  // Initialize an environment for identifier lookup
symbol_operator op{};                                  // Initialize an overridable operator collection
evaluator e{{env}, op};                                // Initialize an evaluator tree-visitor
symbol_table_t result_env{};
for(auto& r : result.declarations)                     // Extract all the declarations
    result_env[r.first] = e.evaluate(r.second.tree);
for(auto& s : result_env)                              // Print the resulting declarations
    std::cout << s.first << " :-> " << s.second << "\n";
if(result.raw_expression)                              // If the expression is just a RHS expression, print it
    std::cout << e.evaluate(result.raw_expression.value()) << "\n";
```

### Using The Z3 Functionality
If enabled, expr includes an integration with the [z3 sat solver](https://github.com/Z3Prover/z3). 
By providing an initial environment, some "unknown"/assignable variables and a boolean expression, 
expr can provide you with an assignment  of the "unknown" variables, such that the boolean expression evaluates to `true`.
This is best explained with an example. The demo binary is a fine place to explore this driver:
```shell
$ ./expr_demo --unknown-environment "a := false; b := false; c := false" \
              --expression "a == !(b || c)" 
```
We ask z3 to find a solution where `a` would be the same value as the expression `!(b || c)`. 
Below is the result of the above query.
```
z3 sat check: (apply these changes to satisfy the expression)
        > a :-> false b
        > b :-> true b
        > c :-> false b
```
There are in fact more than one solution to this query - you could just as well set `c :-> true` and the query would still
be satisfied. 
If you want to iterate over all solutions, you can just append every solution you get to your query with a `!`-operator in front of it, and run the program again.

If you provide a query, where no assignment of the variables would lead to `true`, the driver will exclaim `"unsat"` in the terminal, and the `result` symbol table will be empty:
```shell
$ ./expr_demo --unknown-environment "a := false" \
              --expression "a != a" # not satisfiable
```
```
z3 sat check: (apply these changes to satisfy the expression)
        > unsatisfiable
```
