#ifndef EXPR_ADD_H
#define EXPR_ADD_H
#include "symbol_table.h"
symbol_value_t add(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator+(const symbol_value_t& a, const symbol_value_t& b);
#endif //EXPR_ADD_H
