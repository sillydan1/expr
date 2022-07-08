#ifndef EXPR_ADD_H
#define EXPR_ADD_H
#include "symbol_table.h"
expr::symbol_value_t add(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t operator+(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
#endif //EXPR_ADD_H
