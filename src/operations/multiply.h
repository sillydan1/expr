#ifndef EXPR_MULTIPLY_H
#define EXPR_MULTIPLY_H
#include "symbol_table.h"
expr::symbol_value_t multiply(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t operator*(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
#endif //EXPR_MULTIPLY_H
