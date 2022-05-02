#ifndef EXPR_DIVIDE_H
#define EXPR_DIVIDE_H
#include "symbol_table.h"
symbol_value_t divide(const symbol_value_t& a, const symbol_value_t& b);
symbol_value_t operator/(const symbol_value_t& a, const symbol_value_t& b);
#endif //EXPR_DIVIDE_H
