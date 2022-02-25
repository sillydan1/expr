# expr
A variable environment manipulation expression parser written in C++17

## Example
```
./expr_demo -
a := (1 + 2 + 3 + 4);
b := one + two;
^D
a = 6 i
b = 3 i
```

## Compile
```
mkdir bin && cd bin
cmake ..
make 
```
