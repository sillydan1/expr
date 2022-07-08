#ifndef EXPR_SUBTRACT_H
#define EXPR_SUBTRACT_H
#include "symbol_table.h"
expr::symbol_value_t subtract(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t operator-(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
#endif //EXPR_SUBTRACT_H
