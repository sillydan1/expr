#ifndef EXPR_SUBTRACT_H
#define EXPR_SUBTRACT_H
#include "symbol_table.h"
symbol_value_t subtract(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator-(const symbol_value_t& a, const symbol_value_t& b);
#endif //EXPR_SUBTRACT_H
