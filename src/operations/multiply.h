#ifndef EXPR_MULTIPLY_H
#define EXPR_MULTIPLY_H
#include "symbol_table.h"
symbol_value_t multiply(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator*(const symbol_value_t& a, const symbol_value_t& b);
#endif //EXPR_MULTIPLY_H
