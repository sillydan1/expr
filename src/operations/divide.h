#ifndef EXPR_DIVIDE_H
#define EXPR_DIVIDE_H
#include "symbol_table.h"
expr::symbol_value_t divide(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
expr::symbol_value_t operator/(const expr::symbol_value_t& a, const expr::symbol_value_t& b);
#endif //EXPR_DIVIDE_H
